import os
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Literal, overload

import torch
from torch import nn, optim
from torch.nn import MSELoss
from torch.optim.lr_scheduler import OneCycleLR
from torch.utils.data import DataLoader
from tqdm import tqdm

from data import LichessPositions
from model import HalfKPModel, SimpleModel


@dataclass
class PosInData:
    step: Literal['train', 'validate']
    completed: int = 0
    curr_total_loss: float = 0

    def enter_train(self):
        self.step = 'train'
        self.completed = 0
        self.curr_total_loss = 0

    def enter_validate(self):
        self.step = 'validate'
        self.completed = 0
        self.curr_total_loss = 0

    def add_loss(self, n_loss):
        self.curr_total_loss += n_loss
        self.completed += 1

    @property
    def avg_loss(self):
        return self.curr_total_loss / self.completed

@dataclass
class TrainConfig:
    epochs: int = 3
    curr_epoch: int = 0
    train_positions: int = 1_000_000
    validation_positions: int = 1_000_000
    batch_size: int = 10_000

    encoding: Literal['halfkp', 'simple'] = 'simple'
    minutes_per_checkpoint: int = 10

    @property
    def train_step_count(self) -> int:
        return self.train_positions // self.batch_size

torch.serialization.add_safe_globals([TrainConfig, PosInData])

class Trainer:
    @overload
    def __init__(self, *, config: TrainConfig, data_path: Path | str) -> None: ...
    @overload
    def __init__(self, *, load_path: Path | str, data_path: Path | str) -> None: ...

    def __init__(
        self,
        *,
        config: TrainConfig | None = None,
        load_path: Path | str | None = None,
        data_path: Path | str
    ):
        self.checkpoint_dir = datetime.now().strftime("%B-%d_%H_%M") if load_path is None else os.path.dirname(load_path)
        if config is not None:
            self.config = config
            self.pos_in_data = PosInData('train')
            os.makedirs(self.checkpoint_dir, exist_ok=False)
        else:
            meta = torch.load(load_path)
            self.config = meta['config']
            self.pos_in_data = meta['pos_in_data']

        if self.config.encoding == 'halfkp':
            self.model = HalfKPModel()
        else:
            self.model = SimpleModel()
        self.optimizer = optim.SGD(self.model.parameters())
        self.scheduler = OneCycleLR(self.optimizer, max_lr=.01, steps_per_epoch=self.config.train_step_count,
                                    epochs=self.config.epochs)

        if load_path is not None:
            info = torch.load(load_path)
            self.model.load_state_dict(info['model_state_dict'])
            self.optimizer.load_state_dict(info['optimizer_state_dict'])
            self.scheduler.load_state_dict(info['scheduler_state_dict'])


        train, validate = LichessPositions.create(self.config.train_positions, self.config.validation_positions, self.config.encoding, data_path)
        if load_path is not None:
            if self.pos_in_data.step == 'train':
                train.seek(self.pos_in_data.completed * self.config.batch_size)
            else:
                validate.seek(self.pos_in_data.completed * self.config.batch_size)

        self.train_loader = DataLoader(
            train,
            batch_size=self.config.batch_size,
            num_workers=3
        )
        self.validate_loader = DataLoader(
            validate,
            batch_size=self.config.batch_size,
            num_workers=3
        )

        self.last_checkpoint_time = datetime.now()

    def __call__(self):
        loss_fn = nn.MSELoss()

        while self.config.curr_epoch < self.config.epochs:
            if self.pos_in_data.step == 'train':
                train_loss = self.train_step(loss_fn)
                self.pos_in_data.enter_validate()
                validation_loss = self.validate_step(loss_fn)
            else:
                train_loss = 1
                validation_loss = self.validate_step(loss_fn)

            print(f"Epoch train loss {train_loss}, validation loss {validation_loss}")

            self.config.curr_epoch += 1
            self.pos_in_data.enter_train()

        #self.model.export.save(os.path.join(self.checkpoint_dir, "final.json"))

    def train_step(self, loss_fn: MSELoss) -> float:
        self.model.train()

        for (our, enemy), value in tqdm(self.train_loader):
            pred = self.model(our, enemy).squeeze()

            loss = loss_fn(pred, value.to(torch.float32))

            loss.backward()
            self.optimizer.step()
            self.scheduler.step()
            self.optimizer.zero_grad()

            self.pos_in_data.add_loss(loss.item())
            self.timed_save_checkpoint()

        self.train_loader.dataset.seek(0)

        return self.pos_in_data.avg_loss

    def validate_step(self, loss_fn: MSELoss) -> float:
        self.model.eval()

        for (our, enemy), value in tqdm(self.validate_loader):
            pred = self.model(our, enemy).squeeze()

            loss = loss_fn(pred, value.to(torch.float32))

            self.pos_in_data.add_loss(loss.item())

        self.validate_loader.dataset.seek(0)

        return self.pos_in_data.avg_loss

    def timed_save_checkpoint(self) -> None:
        time_difference = datetime.now() - self.last_checkpoint_time
        minutes_difference = time_difference.total_seconds() / 60

        if self.config.minutes_per_checkpoint < minutes_difference:
            self.save_checkpoint()
            self.last_checkpoint_time = datetime.now()

    def save_checkpoint(self) -> None:
        save_path = os.path.join(self.checkpoint_dir, f"{self.config.curr_epoch}.pth")
        checkpoint = {
            'config': self.config,
            'pos_in_data': self.pos_in_data,
            'model_state_dict': self.model.state_dict(),
            'optimizer_state_dict': self.optimizer.state_dict(),
            'scheduler_state_dict': self.scheduler.state_dict()
        }
        torch.save(checkpoint, save_path)

def main():
    train = Trainer(load_path="December-30_11_12/0.pth", data_path="data/simple-329082547.bin")#config=TrainConfig(train_positions=100_000_000)) #6751it [15:31,  7.25it/s]
    train()

if __name__ == '__main__':
    main()
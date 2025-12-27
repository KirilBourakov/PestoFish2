import os
from datetime import datetime
from pathlib import Path

import torch
from torch import nn, optim
from torch.nn import MSELoss
from torch.optim import Optimizer
from torch.optim.lr_scheduler import ReduceLROnPlateau, OneCycleLR, LRScheduler
from torch.utils.data import DataLoader

from data import Positions
from model import Model

class Trainer:
    def __init__(
        self,
        epochs: int = 10,
        train_positions: int = 1_000_000,
        validation_positions: int = 1_000_000,
        export_path = "model_weights.json",
        load_path: Path | str | None = None
    ):
        self.checkpoint_dir = datetime.now().strftime("%B-%d_%H_%M")
        self.epochs = epochs
        self.train_positions = train_positions
        self.validation_positions = validation_positions
        self.export_path = export_path
        self.load_path = load_path

        os.makedirs(self.checkpoint_dir, exist_ok=False)

    def __call__(self):
        train, validate = Positions.create(self.train_positions, self.validation_positions)

        batch_size=1024
        train_loader = DataLoader(
            train,
            batch_size=batch_size,
            num_workers=3
        )
        validate_loader = DataLoader(
            validate,
            batch_size=batch_size,
            num_workers=3
        )

        model = Model()
        optimizer = optim.SGD(model.parameters())
        scheduler = OneCycleLR(optimizer, max_lr=.01, steps_per_epoch=(self.train_positions // batch_size), epochs=self.epochs)

        loss_fn = nn.MSELoss()
        epoch = 0
        train_loss_history = []
        validation_loss_history = []
        if self.load_path is not None:
            epoch, train_loss_history, validation_loss_history = self.load_checkpoint(self.load_path, model, optimizer)

        for i in range(epoch, self.epochs, 1):
            train_loss = self.train_step(model, train_loader, loss_fn, optimizer, scheduler)
            validation_loss = self.validate_step(model, validate_loader, loss_fn)

            train_loss_history.append(train_loss)
            validation_loss_history.append(validation_loss)
            if min(validation_loss_history) == validation_loss:
                self.save_checkpoint(model, optimizer, i, train_loss_history, validation_loss_history)

            print(f"Epoch train loss {train_loss}, validation loss {validation_loss}")

        save_path = os.path.join(self.checkpoint_dir, self.export_path)
        model.export.save(save_path)

    @staticmethod
    def train_step(model: Model, data: DataLoader, loss_fn: MSELoss, optimizer: Optimizer, scheduler: LRScheduler) -> float:
        batches, epoch_loss = 0, 0
        model.train()
        for (our, enemy), value in data:
            pred = model(our.to(torch.float32), enemy.to(torch.float32)).squeeze()

            loss = loss_fn(pred, value.to(torch.float32))

            loss.backward()
            optimizer.step()
            scheduler.step()
            optimizer.zero_grad()

            epoch_loss += loss.item()
            batches += 1
        return epoch_loss / batches

    @staticmethod
    def validate_step(model: Model, data: DataLoader, loss_fn: MSELoss) -> float:
        batches, epoch_loss = 0, 0
        model.eval()
        for (our, enemy), value in data:
            pred = model(our.to(torch.float32), enemy.to(torch.float32)).squeeze()

            loss = loss_fn(pred, value.to(torch.float32))

            epoch_loss += loss.item()
            batches += 1
        return epoch_loss / batches

    def save_checkpoint(self, model: Model, optimizer: Optimizer, epoch: int, train_loss_history: list[float], validation_loss_history: list[float]) -> None:
        save_path = os.path.join(self.checkpoint_dir, f"{epoch}-{validation_loss_history[-1]}.pth")
        checkpoint = {
            'epoch': epoch,
            'model_state_dict': model.state_dict(),
            'optimizer_state_dict': optimizer.state_dict(),
            'train_loss_history': train_loss_history,
            'validation_loss_history': validation_loss_history
        }
        torch.save(checkpoint, save_path)

    @staticmethod
    def load_checkpoint(filepath: str | Path, model: Model, optimizer: Optimizer) -> tuple[int, list[float], list[float]]:
        checkpoint = torch.load(filepath)
        model.load_state_dict(checkpoint['model_state_dict'])
        optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
        epoch = checkpoint['epoch']
        train_loss_history = checkpoint['train_loss_history']
        validation_loss_history = checkpoint['validation_loss_history']
        return epoch + 1, train_loss_history, validation_loss_history

def main():
    train = Trainer()
    train()

if __name__ == '__main__':
    main()
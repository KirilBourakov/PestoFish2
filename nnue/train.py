import os
from datetime import datetime
from pathlib import Path

import torch
from torch import nn, optim
from torch.optim import Optimizer
from torch.utils.data import DataLoader

from data import Positions
from model import Model

class Trainer:
    def __init__(
        self,
        epochs: int = 10,
        positions: int = 1_000_000,
        export_path = "model_weights.json",
        load_path: Path | str | None = None
    ):
        self.checkpoint_dir = datetime.now().strftime("%B-%d_%H_%M")
        self.epochs = epochs
        self.positions = positions
        self.export_path = export_path
        self.load_path = load_path

        os.makedirs(self.checkpoint_dir, exist_ok=False)

    def __call__(self):
        loader = DataLoader(
            Positions(limit=self.positions),
            batch_size=1024,
            num_workers=3
        )

        model = Model()
        optimizer = optim.SGD(model.parameters(), lr=0.01)
        loss_fn = nn.MSELoss()
        epoch = 0
        if self.load_path is not None:
            epoch = self.load_checkpoint(self.load_path, model, optimizer)

        for i in range(epoch, self.epochs, 1):
            batches, epoch_loss = 0,0
            for (our, enemy), value in loader:
                pred = model(our.to(torch.float32), enemy.to(torch.float32)).squeeze()

                loss = loss_fn(pred, value.to(torch.float32))

                loss.backward()
                optimizer.step()
                optimizer.zero_grad()

                epoch_loss += loss.item()
                batches += 1

            epoch_loss /= batches
            self.save_checkpoint(model, optimizer, i, epoch_loss)
            print(f"Epoch loss {epoch_loss}")

        save_path = os.path.join(self.checkpoint_dir, self.export_path)
        model.export.save(save_path)

    def save_checkpoint(self, model: Model, optimizer: Optimizer, epoch: int, loss: float) -> None:
        save_path = os.path.join(self.checkpoint_dir, f"{epoch}-{loss}.pth")
        checkpoint = {
            'epoch': epoch,
            'model_state_dict': model.state_dict(),
            'optimizer_state_dict': optimizer.state_dict(),
            'loss': loss,
        }
        torch.save(checkpoint, save_path)

    @staticmethod
    def load_checkpoint(filepath: str | Path, model: Model, optimizer: Optimizer) -> int:
        checkpoint = torch.load(filepath)
        model.load_state_dict(checkpoint['model_state_dict'])
        optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
        epoch = checkpoint['epoch']
        return epoch + 1

if __name__ == '__main__':
    train = Trainer()
    train()
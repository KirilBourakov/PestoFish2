import os
from datetime import datetime

import torch
from torch import nn, optim
from torch.utils.data import DataLoader

from data import Positions
from model import Model

class Trainer:
    def __init__(self, epochs: int = 10, positions: int = 1_000_000, export_path = "model_weights.json"):
        self.checkpoint_dir = datetime.now().strftime("%B-%d_%H_%M")
        self.epochs = epochs
        self.positions = positions
        self.curr_checkpoint = 1
        self.export_path = export_path

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

        for i in range(self.epochs):
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
            self.save_checkpoint(model, epoch_loss)
            print(f"Epoch loss {epoch_loss}")

        save_path = os.path.join(self.checkpoint_dir, self.export_path)
        model.export.save(save_path)

    def save_checkpoint(self, model: Model, loss: float):
        save_path = os.path.join(self.checkpoint_dir, f"{self.curr_checkpoint}-{loss}.pth")
        torch.save(model.state_dict(), save_path)
        self.curr_checkpoint += 1


if __name__ == '__main__':
    train = Trainer()
    train()
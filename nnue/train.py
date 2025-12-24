import torch
from torch import nn, optim
from torch.utils.data import DataLoader

from data import Positions
from model import Model

if __name__ == '__main__':
    data = Positions()
    loader = DataLoader(data)

    model = Model()
    optimizer = optim.SGD(model.parameters(), lr=0.01)

    loss_fn = nn.MSELoss()
    for i in range(100):
        for (our, enemy), value in loader:
            pred = model(our.to(torch.float32), enemy.to(torch.float32))


            loss = loss_fn(pred, value.to(torch.float32))

            loss.backward()
            optimizer.step()
            optimizer.zero_grad()

            print(f"Loss: {loss.item():.4f}")
import torch
from torch import nn, optim
from torch.utils.data import DataLoader

from data import Positions
from model import Model

if __name__ == '__main__':
    data = Positions(limit=1000000)
    loader = DataLoader(data, batch_size=1024, num_workers=3)

    model = Model()
    optimizer = optim.SGD(model.parameters(), lr=0.01)

    loss_fn = nn.MSELoss()
    for i in range(100):
        batches = 0
        epoch_loss = 0
        for (our, enemy), value in loader:
            pred = model(our.to(torch.float32), enemy.to(torch.float32)).squeeze()

            loss = loss_fn(pred, value.to(torch.float32))

            loss.backward()
            optimizer.step()
            optimizer.zero_grad()

            epoch_loss += loss.item()
            batches += 1
        print(f"Epoch loss {epoch_loss / batches}")
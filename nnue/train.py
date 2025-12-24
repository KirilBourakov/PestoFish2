from torch.utils.data import DataLoader

from data import Positions

if __name__ == '__main__':
    data = Positions()
    loader = DataLoader(data)
    for data in loader:
        (p1, p2), value = data
        print(data)
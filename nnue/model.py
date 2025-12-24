import torch
from torch import nn, Tensor

INPUT_BITS = 40960

class CReLU(nn.Module):
    def __init__(self):
        super(CReLU, self).__init__()
    def forward(self, x):
        return torch.clamp(x, 0, 1)

class Model(nn.Module):
    def __init__(self) -> None:
        super().__init__()
        self.feature_transformer = nn.Linear(INPUT_BITS, 16)

        self.layer_1 = nn.Linear(16 * 2, 32)

        self.output = nn.Sequential(
            CReLU(),
            nn.Linear(32, 32),
            CReLU(),
            nn.Linear(32, 1)
        )

    def forward(self, our_color: Tensor, enemy_color: Tensor) -> Tensor:
        """
        Forward pass:
        Args:
            our_color: Encoding from our perspective
            enemy_color: Encoding from enemy perspective
        Returns:
            Score prediction
        """
        our_processed = self.feature_transformer(our_color)
        enemy_processed = self.feature_transformer(enemy_color)

        combined = torch.cat([our_processed, enemy_processed], dim=-1)

        return self.output(self.layer_1(combined))
from typing import Annotated, Any, TypeVar

import numpy as np
import torch
from pydantic import BaseModel, BeforeValidator, PlainSerializer, ConfigDict
from torch import nn, Tensor
import numpy.typing as npt
from torch.nn import Tanh

from data import HALF_KP_ENCODING_SIZE
from parse import SIMPLE_FEATURES, MAX_SCORE


def val(v: Any) -> npt.NDArray[np.generic]:
    return np.asarray(v)

def serialize(v: npt.NDArray[np.generic]) -> list[Any]:
    return v.tolist()

type PydanticNDArray[Tnum: np.generic] = Annotated[
    npt.NDArray[Tnum],
    BeforeValidator(val),
    PlainSerializer(serialize, return_type=list)
]

class CReLU(nn.Module):
    def __init__(self):
        super().__init__()
    def forward(self, x):
        return torch.clamp(x, 0, 1)


class SimpleModel(nn.Module):
    def __init__(self, l1_size=512):
        super(SimpleModel, self).__init__()
        self.feature_transformer = nn.Linear(768, l1_size)
        self.out = nn.Linear(l1_size*2, 1)

    def forward(self, stm_features: Tensor, non_stm_features: Tensor) -> Tensor:
        """
        stm_features: Features from the perspective of the player whose turn it is.
        non_stm_features: Features from the perspective of the opponent.
        """
        acc_stm = self.feature_transformer(stm_features.float())
        acc_non_stm = self.feature_transformer(non_stm_features.float())
        combined = torch.cat([acc_stm, acc_non_stm], dim=1)
        combined = torch.clamp(combined, 0.0, 1.0)
        return self.out(combined)

def nnue_loss(pred: torch.Tensor, target: torch.Tensor) -> torch.Tensor:
    model_eval = (pred * 600.0 / 361).sigmoid()
    target_eval = (target / 410).sigmoid()
    return (target_eval  - model_eval).square().mean()

class HalfKPModel(nn.Module):
    def __init__(self) -> None:
        super().__init__()
        self.feature_transformer = nn.Linear(HALF_KP_ENCODING_SIZE, 16)

        self.layer_1 = nn.Linear(16 * 2, 32)

        self.output = nn.Sequential(
            Clamp(),
            nn.Linear(32, 32),
            Clamp(),
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
        our_processed = self.feature_transformer(our_color.float())
        enemy_processed = self.feature_transformer(enemy_color.float())

        combined = torch.cat([our_processed, enemy_processed], dim=-1)

        return self.output(self.layer_1(combined))
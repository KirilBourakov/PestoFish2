from typing import Annotated, Any, TypeVar

import numpy as np
import torch
from pydantic import BaseModel, BeforeValidator, PlainSerializer, ConfigDict
from torch import nn, Tensor
import numpy.typing as npt

from data import ENCODING_SIZE

def val(v: Any) -> npt.NDArray[np.generic]:
    return np.asarray(v)

def serialize(v: npt.NDArray[np.generic]) -> list[Any]:
    return v.tolist()

type PydanticNDArray[Tnum: np.generic] = Annotated[
    npt.NDArray[Tnum],
    BeforeValidator(val),
    PlainSerializer(serialize, return_type=list)
]

class WeightModel(BaseModel):
    model_config = ConfigDict(arbitrary_types_allowed=True)

    feature_transformer: PydanticNDArray[np.float32]
    layer_1: PydanticNDArray[np.float32]
    layer_2: PydanticNDArray[np.float32]
    layer_3: PydanticNDArray[np.float32]

    def save(self, path: str) -> None:
        json_string = self.model_dump_json(indent=4)
        with open(path, 'w') as f:
            f.write(json_string)


class CReLU(nn.Module):
    def __init__(self):
        super().__init__()
    def forward(self, x):
        return torch.clamp(x, 0, 1)

class Model(nn.Module):
    def __init__(self) -> None:
        super().__init__()
        self.feature_transformer = nn.Linear(ENCODING_SIZE, 16)

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

    @property
    def export(self) -> WeightModel:
        return WeightModel(
            feature_transformer=self.feature_transformer.weight.to("cpu").detach().numpy(),
            layer_1=self.layer_1.weight.to("cpu").detach().numpy(),
            layer_2=self.output[1].weight.to("cpu").detach().numpy(),
            layer_3=self.output[3].weight.to("cpu").detach().numpy(),
        )
import sys
from pathlib import Path
from typing import Annotated, Any, TypeVar

import numpy as np
import torch
from pydantic import BaseModel, BeforeValidator, PlainSerializer, ConfigDict
import numpy.typing as npt

from model import SimpleModel
from train import TrainConfig, PosInData


def val(v: Any) -> npt.NDArray[np.generic]:
    return np.asarray(v)

def serialize(v: npt.NDArray[np.generic]) -> list[Any]:
    return v.tolist()

type PydanticNDArray[Tnum: np.generic] = Annotated[
    npt.NDArray[Tnum],
    BeforeValidator(val),
    PlainSerializer(serialize, return_type=list)
]

sys.modules['__main__'].TrainConfig = TrainConfig
sys.modules['__main__'].PosInData = PosInData

class JsonEncoding(BaseModel):
    model_config = ConfigDict(arbitrary_types_allowed=True)

    accumulator_weights: PydanticNDArray[np.int16]
    accumulator_biases: PydanticNDArray[np.int16]
    output_weights: PydanticNDArray[np.int16]
    output_bias: int

    @staticmethod
    def from_model(model: SimpleModel) -> "JsonEncoding":
        L1_SCALE = 255
        L2_SCALE = 64

        print(f"Input weights: \n\t {model.feature_transformer.weight.shape}")
        print(f"Input Bias: \n\t {model.feature_transformer.bias.shape}")
        print(f"Output weights: \n\t {model.out.weight.shape}")
        print(f"Output bias: \n\t {model.out.bias.shape}")

        return JsonEncoding(
            accumulator_weights=np.round(model.feature_transformer.weight.detach().numpy() * L1_SCALE).astype(np.int16),
            accumulator_biases=np.round(model.feature_transformer.bias.detach().numpy() * L1_SCALE).astype(np.int16),
            output_weights=np.round(model.out.weight.detach().numpy() * L2_SCALE).astype(np.int16),
            output_bias=np.round(model.out.bias.detach().numpy() * L1_SCALE * L2_SCALE).astype(np.int16)[0],
        )

    def write(self, path: str | Path) -> None:
        with open(path, "w") as f:
            f.write(self.model_dump_json(indent=2))

def main(path: str | Path):
    data = torch.load(path, weights_only=False)
    model = SimpleModel()
    model.load_state_dict(data['model_state_dict'])

    print(f"L1 W: min={model.feature_transformer.weight.min()}, max={model.feature_transformer.weight.max()}")
    print(f"L1 B: min={model.feature_transformer.bias.min()}, max={model.feature_transformer.bias.max()}")
    print(f"L2 W: min={model.out.weight.min()}, max={model.out.weight.max()}")
    print(f"L2 B: min={model.out.bias.min()}, max={model.out.bias.max()}")

    JsonEncoding.from_model(model).write("out.json")

if __name__ == "__main__":
    main("December-31_22_00/4 - 0.0410061841982771.pth")
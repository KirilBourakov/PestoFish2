from collections.abc import Callable
from .material import material_eval

common: list[Callable[[list[list[str]]], int]] = [material_eval]
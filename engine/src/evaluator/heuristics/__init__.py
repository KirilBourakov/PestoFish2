from collections.abc import Callable
from engine.src.evaluator.heuristics.material import material_eval

common: list[Callable[[list[list[str]]], int]] = [material_eval]
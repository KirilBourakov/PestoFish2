from collections.abc import Callable
from engine.src.evaluator.heuristics.material import material_eval

independant: list[Callable[[str, tuple[int, int], bool], int]] = [material_eval]
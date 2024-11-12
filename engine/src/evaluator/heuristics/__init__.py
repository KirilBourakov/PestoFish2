from collections.abc import Callable
from engine.src.constants.engineTypes import boardType
from engine.src.evaluator.heuristics.material import material_eval
from engine.src.evaluator.heuristics.piece_position import piece_position
from engine.src.evaluator.heuristics.piece_mobility import piece_mobility

independant: list[Callable[[str, tuple[int, int], bool], int]] = [material_eval, piece_position]

dependant: list[Callable[[boardType, bool], int]] = [piece_mobility]
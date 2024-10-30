from collections.abc import Callable
from engine.src.constants.constants import MIDDLE_GAME, END_GAME, PAWN, ROOK, BISHOP, KNIGHT, QUEEN
from heuristics import common

class Evaluator():
    def __init__(self) -> None:
        self.current_game_type: str = MIDDLE_GAME

        # heuristics
        self.common_heuristics: list[Callable[[list[list[str]]], int]] = common

    def eval(self, board: list[list[str]]) -> int:
        eval_estimate: int = 0

        heuristic: Callable[[list[list[str]]], int]
        for heuristic in self.common_heuristics:
            eval_estimate += heuristic(board)

        return eval_estimate
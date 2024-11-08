from collections.abc import Callable
from engine.src.helpers.helpers import flip
from engine.src.helpers.board_analysis import sight_on_square, find_king
from engine.src.constants.static import MIDDLE_GAME, END_GAME, PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, BLACK
from .heuristics import common
from engine.src.generator.generator import Generator

class Evaluator():
    def __init__(self) -> None:
        '''Creates an Evaluator, which is used to evaluate positions'''
        self.current_game_type: str = MIDDLE_GAME

        # heuristics
        self.common_heuristics: list[Callable[[list[list[str]]], int]] = common

    def eval(self, board: list[list[str]], game_over: bool) -> float:
        '''Evaluates a given board. Returns a score in centipawns (1/100 of a pawn).'''

        if game_over:
            for color in [WHITE, BLACK]:
                enemy = flip(color)
                generator = Generator()

                # see if the enemy king is in check, but has no moves
                moves = generator.get_moves(board, find_king(board, enemy)) 
                eyes_on_king: dict[str, list[tuple[int, int]]] = sight_on_square(board, find_king(board, color))
                king_in_check: bool = len(eyes_on_king[enemy]) > 0
                # a king is in checkmate
                if king_in_check: 
                    if len(moves) == 0:
                        return float('inf') if color == WHITE else float('-inf')
            return 0

        eval_estimate: float = 0.0
        heuristic: Callable[[list[list[str]]], int]
        for heuristic in self.common_heuristics:
            eval_estimate += heuristic(board)

        return eval_estimate
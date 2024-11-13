import tensorflow as tf
import numpy as np
import numpy.typing as npt
from collections.abc import Callable
from engine.src.helpers.helpers import flip
from engine.src.helpers.square_analysis import get_color, get_type, is_empty
from engine.src.helpers.board_analysis import sight_on_square, find_king
from engine.src.constants.engineTypes import boardType, MoveType
from engine.src.constants.static import MIDDLE_GAME, END_GAME, PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, BLACK, KING
from engine.src.evaluator.heuristics import independant, dependant
from engine.src.generator.generator import Generator

class Evaluator():
    def __init__(self) -> None:
        '''Creates an Evaluator, which is used to evaluate positions'''
        # heuristics
        self.board_independant_heuristics: list[Callable[[str, tuple[int, int], bool], int]] = independant
        self.board_dependant_heuristics: list[Callable[[boardType, bool], int]] = dependant
        self.model = tf.keras.models.load_model('m.keras')
        self.code_map = {
                        '  ': 0,
                        'wr': 490,
                        'Wr': 505,
                        'wk': 300,
                        'wb': 325,
                        'wq': 900,
                        'wK': 1000,
                        'WK': 1001,
                        'we': 1,
                        'wp': 100,

                        'br': -490,
                        'Br': -505,
                        'bk': -300,
                        'bb': -325,
                        'bq': -900,
                        'bK': -1000,
                        'BK': -1001,
                        'be': -1,
                        'bp': -100,
                    }

    def net_eval(self, board: boardType, game_over: bool) -> float:
        if game_over:
            generator: Generator = Generator()
            for color in [WHITE, BLACK]:
                enemy: str = flip(color)
                
                # see if the enemy king is in check, but has no moves
                enemy_king_pos = find_king(board, enemy)
                moves: list[MoveType] = generator.get_moves(board, enemy_king_pos) 
                eyes_on_king: dict[str, list[tuple[int, int]]] = sight_on_square(board, enemy_king_pos)
                king_in_check: bool = len(eyes_on_king[color]) > 0

                # a king is in checkmate
                if king_in_check: 
                    if len(moves) == 0:
                        return float('inf') if color == WHITE else float('-inf')
            return 0
        nb = self.parse_board(board)
        try:
            val = self.model.predict(nb)
            return val
        except:
            for row in board:
                print("board", row)
            print(nb)
            raise Exception('error')
        

    def parse_board(self, board: boardType):
        final = []
        for y in range(len(board)):
            for x in range(len(board)):
                final.append(self.code_map[board[y][x]])
    
        return np.array(final)

    def eval(self, board: boardType, game_over: bool) -> float:
        '''Evaluates a given board. Returns a score in centipawns (1/100 of a pawn).'''

        if game_over:
            generator: Generator = Generator()
            for color in [WHITE, BLACK]:
                enemy: str = flip(color)
                
                # see if the enemy king is in check, but has no moves
                enemy_king_pos = find_king(board, enemy)
                moves: list[MoveType] = generator.get_moves(board, enemy_king_pos) 
                eyes_on_king: dict[str, list[tuple[int, int]]] = sight_on_square(board, enemy_king_pos)
                king_in_check: bool = len(eyes_on_king[color]) > 0

                # a king is in checkmate
                if king_in_check: 
                    if len(moves) == 0:
                        return float('inf') if color == WHITE else float('-inf')
            return 0

        eval_estimate: float = 0.0
        is_endgame: bool = self.get_is_endgame(board)

        for y, row in enumerate(board):
            for x, square in enumerate(row):
                for board_independant_heuristic in self.board_independant_heuristics:
                    eval_estimate += board_independant_heuristic(square, (x,y), is_endgame) 
        # TODO: very expensive, find a better way. Espically something like piece_mobility, which adds 2,3x to eval move choice time.
        for heuristic in self.board_dependant_heuristics:
            eval_estimate += heuristic(board, is_endgame)

        return eval_estimate
    
    def get_is_endgame(self, board: boardType) -> bool:
        count = {
            BLACK: 0,
            WHITE: 0
        }
        for row in board:
            for square in row:    
                if not is_empty(square):
                    if not get_type(square) == KING and not get_type(square) == PAWN:
                        count[get_color(square)] += 1
        return count[BLACK] <= 4 and count[WHITE] <= 4
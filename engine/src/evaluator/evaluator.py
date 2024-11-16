import tensorflow as tf
import time
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


model = tf.lite.Interpreter('lite.tflite')
model.allocate_tensors()
inp = model.get_input_details()[0] 
outp = model.get_output_details()[0]

class Evaluator():
    def __init__(self) -> None:
        '''Creates an Evaluator, which is used to evaluate positions'''
        # heuristics
        self.board_independant_heuristics: list[Callable[[str, tuple[int, int], bool], int]] = independant
        self.board_dependant_heuristics: list[Callable[[boardType, bool], int]] = dependant

        # tf.keras.models.load_model('m.keras', compile=True)
        self.index = {
            'bK': 0,
            'br': 2,
            'bq': 1,
            'bb': 3,
            'bk': 4,
            'bp': 5,

            'wK': 6,
            'wr': 8,
            'wq': 7,
            'wb': 9,
            'wk': 10,
            'wp': 11,

            '  ': 12,
            'we': 12,
            'be': 12
        }

    def net_eval(self, board: boardType, game_over: bool, move_color: str) -> float:
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

        model.set_tensor(inp['index'], self.parse_board(board, move_color))
        model.invoke()
        eval_estimate += (model.get_tensor(outp['index']) / 2)
        return eval_estimate

    def parse_board(self, board: boardType, move_color: str):
        final_board = []
        for i in range(8):
            final_board.append([])
        for i in range(8):
            for j in range(8):
                final_board[i].append([0]*13)

        for y, row in enumerate(board):
            for x, square in enumerate(row):
                final_board[y][x][self.index[square[0].lower()+square[1]]] = 1
        x = np.array(final_board, dtype=np.float32).reshape(-1, 8, 8, 13)
        return x

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
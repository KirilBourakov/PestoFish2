import copy
import math
from .constants.constants import BLACK, WHITE, KING, EMPTY, EN_PASSENT
from .constants.types import MoveType
from .helpers.square_analysis import get_color, get_type
from .helpers.board_analysis import sight_on_square
from .helpers.helpers import flip
from .generator.generator import Generator
from .evaluator.evaluator import Evaluator

class engine():
    def __init__(self) -> None:
        self.generator: Generator = Generator()
        self.evaluator: Evaluator = Evaluator()

        self.kingPos: dict[str, tuple[int, int]] = {BLACK: (-10,-10), WHITE: (-10,-10)}


    def accept_board(self, boardStr: str) -> list[list[str]]:
        '''Takes in a boardStr and parses the board in a way the engine can understand.
        Also extracts important features about the board'''
        split: list[str] = boardStr.split('/')
        self.fifty_move_rule_counter: int = int(split.pop())
        self.move_counter: int = int(split.pop())

        self.board: list[list[str]] = []

        for row in split:
            row = row.strip()
            s = row.split(" ")
            f_row = []
            for grid in s:
                f_row.append(grid.replace("--", "  "))
            self.board.append(f_row)
            
        for y, board_row in enumerate(self.board):
            for x, square in enumerate(board_row):
                if (get_type(square) == KING):
                    self.kingPos[get_color(square)] = (x,y)

        return self.board
    
    def get_best_move(self) -> MoveType:
        '''Gets the engine's best guess at what a move is.'''
        current_color = self.to_move(self.move_counter)
        possible_moves = self.generator.get_moves(self.board, self.kingPos[current_color])
        value_moves: list[tuple[MoveType, float]] = []
        
        for move in possible_moves:
            moveVal: float = self.value(self.board, move, 1)
            value_moves.append((move, moveVal))
        return self.get_best(value_moves, current_color)

    def value(self, board: list[list[str]], move: MoveType, currDepth, Maxdepth=3) -> float:
        '''Estimates the value of a move using evaluator and MINIMAX. Currently unfinished.'''
        new_pos: list[list[str]] = self.result(board, move['original'], move['new'])
        piece_moved: str = board[move['original'][1]][move['original'][0]]
        color_just_moved: str = get_color(piece_moved)

        # base cases
        if currDepth > Maxdepth:
            return self.evaluator.eval(new_pos)
        # the position is terminal
        terminal: int = self.is_termainal(new_pos, color_just_moved)
        if terminal != -1:
            return self.get_terminal_value(terminal, color_just_moved)
        
        possible_moves: list[MoveType] = self.generator.get_moves(new_pos, self.kingPos[flip(color_just_moved)])[:3]
        values: list[float] = []
        for move in possible_moves:
            values.append(self.value(self.board, move, currDepth+1))
        return self.get_best_val(values, flip(color_just_moved))

    def get_terminal_value(self, terminal_key, color) -> float:
        '''returns the value for a terminal state given a nonnegative terminal key'''
        if terminal_key == 0:
            return 0
        return float('inf') if get_color(color) == WHITE else float('-inf')

    def get_best_val(self, input: list[float], color: str) -> float:
        if color == BLACK:
            return min(input)
        return max(input)

    def get_best(self, input: list[tuple[MoveType, float]], color: str) -> MoveType:
        '''Given a list of moves and their values, return the best move for a specific color'''
        if color == BLACK:
            return min(input, key=lambda x: x[1])[0]
        return max(input, key=lambda x: x[1])[0]

    def to_move(self, turn_count: int) -> str:
        '''gets who's move it is'''
        if turn_count % 2 == 0:
            return WHITE
        return BLACK
    
    def is_termainal(self, board: list[list[str]], last_move_color: str) -> int:
        '''Returns if the game is over. An int indicates the result. 
        0 for stalemate, 1 for victory, -1 for not terminal
        This method only checks if the last move resulted in a terminal position

        Keyword arguements:
        \t board - the board 
        \t the color that made the move
        '''
        enemy = flip(last_move_color)
        moves = self.generator.get_moves(board, self.kingPos[enemy])

        king_in_check: bool = len(sight_on_square(board, self.kingPos[enemy])[last_move_color]) > 0

        # checkmate
        if king_in_check: 
            if len(moves) == 0:
                return 1

        # stalemate/draw
        if self.fifty_move_rule_counter / 2 >= 50 or len(moves) == 0:
            return 0
        return -1
    
    def result(self, board: list[list[str]], oldPos: tuple[int, int], newPos: tuple[int, int]) -> list[list[str]]:
        '''Simulates a board position
        
        Keyword arguements:
        \t board - the board 
        \t oldPos - the old position of the piece
        \t newPos - the new position of the piec
        '''
        new_board: list[list[str]] = copy.deepcopy(board)
        new_board[newPos[1]][newPos[0]] = new_board[oldPos[1]][oldPos[0]]
        new_board[oldPos[1]][oldPos[0]] = EMPTY

        # enpassent
        color = get_color(board[oldPos[1]][oldPos[0]])
        if get_type(board[newPos[1]][newPos[0]]) == EN_PASSENT:
            offset = 1 if color == BLACK else -1
            new_board[newPos[1]+offset][newPos[0]] = EMPTY
        
        # castling (if the king is moving more then 1 square, it must be castling)
        delta_x = abs(oldPos[0] - newPos[0])
        piece_type = board[oldPos[1]][oldPos[0]]
        if piece_type == KING and delta_x > 1:
            if newPos[0] == 2:
                new_board[newPos[1]][0] = EMPTY
                new_board[newPos[1]][newPos[0]+1] = board[newPos[1]][0]
            elif newPos[0] == 6:
                new_board[newPos[1]][0] = EMPTY
                new_board[newPos[1]][newPos[0]+1] = board[newPos[0]][0]

        return new_board

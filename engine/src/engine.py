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
        self.transposeTable: dict[str, float] = {}


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
            new_pos: list[list[str]] = self.result(self.board, move)
            pos_val: float = self.value(new_pos, current_color)
            value_moves.append((move, pos_val))
            self.transposeTable[str(new_pos)] = pos_val
        return self.get_best(value_moves, current_color)     

    def value(self, pos: list[list[str]], perspective: str, currDepth: int = 1, 
            Maxdepth: int=4, max_val:float=float('-inf'), min_val:float=float('-inf')) -> float:
        '''Estimates the value of a move using evaluator and MINIMAX. Currently unfinished.'''
        # TODO: engine seems to be calculating from wrong perspective/Something else fundementally wrong
        # TODO: taking far too long

        # base cases
        if str(pos) in self.transposeTable:
            return self.transposeTable[str(pos)]
        if currDepth > Maxdepth:
            print(self.evaluator.eval(pos))
            return self.evaluator.eval(pos)
        # the position is terminal
        terminal_key: int = self.is_termainal(pos)
        if terminal_key != -2:
            return self.get_terminal_value(terminal_key)
        
        # get all the possible moves
        possible_moves: list[MoveType] = self.generator.get_moves(pos, self.find_king(pos, flip(perspective)))
        # initalize dummy values for the best_value
        best_value = float('-inf') if perspective == WHITE else float('inf')
        # for every move
        for move in possible_moves:
            new_pos: list[list[str]] = self.result(self.board, move)
            # get the value of the new position
            pos_val = self.value(new_pos, flip(perspective), currDepth=currDepth+1, max_val=max_val, min_val=min_val)
            # update the value as needed
            best_value = self.get_best_val([best_value, pos_val], perspective)

            # keep track of max and min
            if perspective == WHITE:
                max_val = max(max_val, pos_val)
            else:
                min_val = min(min_val, pos_val)
            # prune
            if min_val <= max_val:
                break
            
        return best_value

    def find_king(self, board: list[list[str]], color: str):
        for y, board_row in enumerate(board):
            for x, square in enumerate(board_row):
                if (get_type(square) == KING):
                    if get_color(square) == color:
                        return (x,y)
        raise IndexError('No King Index Found')

    def get_terminal_value(self, terminal_key) -> float:
        '''returns the value for a terminal state given a terminal key'''
        if terminal_key == 0:
            return 0
        return float('inf') if terminal_key == 1 else float('-inf')

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
    
    def is_termainal(self, board: list[list[str]]) -> int:
        '''Returns if the game is over. An int indicates the result. 
        0 for stalemate or draw, 1 for white victory, -1 for black victory, -2 for not terminal
        This method only checks if the last move resulted in a terminal position

        Keyword arguements:
        \t board - the board 
        \t the color that made the move
        '''
        for color in [WHITE, BLACK]:
            enemy = flip(color)
            moves = self.generator.get_moves(board, self.find_king(board, enemy))
            eyes_on_king: dict[str, list[tuple[int, int]]] = sight_on_square(board, self.kingPos[color])
            king_in_check: bool = len(eyes_on_king[enemy]) > 0
            # a king is in checkmate
            if king_in_check: 
                if len(moves) == 0:
                    return 1 if enemy == WHITE else -1
            # stalemate
            if len(moves) == 0:
                return 0

        # draw by fifty move rule
        if self.fifty_move_rule_counter / 2 >= 50:
            return 0
        return -2
    
    def result(self, board: list[list[str]], move: MoveType) -> list[list[str]]:
        # TODO: handle placing of EP flag
        '''Simulates a board position
        
        Keyword arguements:
        \t board - the board 
        \t move - a dict of moveType, representing the move
        '''
        oldx, oldy = move['original']
        newx, newy = move['new']
        new_board: list[list[str]] = copy.deepcopy(board)
        
        new_board[newy][newx] = new_board[oldy][oldx]
        new_board[newy][newx] = new_board[newy][newx][0].lower() + new_board[newy][newx][1]
        new_board[oldy][oldx] = EMPTY

        if move['promotion'] != '':
            new_board[newy][newx] = new_board[newy][newx][0].lower() + move['promotion']

        # enpassent
        color = get_color(board[oldy][oldx])
        if get_type(board[newy][newx]) == EN_PASSENT:
            offset = 1 if color == BLACK else -1
            new_board[newy+offset][newx] = EMPTY
        
        # castling (if the king is moving more then 1 square, it must be castling)
        delta_x = abs(oldx - newx)
        piece_type = board[oldy][oldx]
        if piece_type == KING and delta_x > 1:
            if newx == 2:
                new_board[newy][0] = EMPTY
                new_board[newy][newx+1] = board[newy][0]
            elif newx == 6:
                new_board[newy][0] = EMPTY
                new_board[newy][newx+1] = board[newx][0]

        return new_board

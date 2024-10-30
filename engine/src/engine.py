import copy
from .constants.constants import BLACK, WHITE, KING, EMPTY
from .helpers.square_analysis import get_color, get_type
from .helpers.board_analysis import sight_on_square
from .helpers.helpers import flip
from .generator.generator import Generator
from .evaluator.evaluator import Evaluator

class engine():
    def __init__(self):
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
        return new_board

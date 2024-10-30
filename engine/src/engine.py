from engine.src.constants.constants import BLACK, WHITE, KING
from engine.src.helpers.square_analysis import get_color, get_type

class engine():
    def __init__(self):
        self.generator = None #should contain a class that is used to generate moves
        self.evaluator = None #should contain a class that evaluates a given move position

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
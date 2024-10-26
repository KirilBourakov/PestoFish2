from .moves import Moves

class generator():
    def __init__(self):
        self.move_manager: Moves = Moves()

    def get_moves(self, board: list[list[str]]) -> list[dict[str, int]]:
        '''given a board state return a list of possible moves
        
        thes method returns a list of dictionaries that repersent moves. Each dict has the following key value pairs:
        \t old_x the original x of the piece
        \t old_y the original y of the piece
        \t new_x the new x of the piece
        \t new_y the new y of the piece
        \t type the move type (forcing moves are given a higher value)
        '''
        pass
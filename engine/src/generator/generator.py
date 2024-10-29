from .moves import Moves
from engine.src.constants.constants import BLACK, WHITE, KING, EMPTY, SHORT_CASTLE, LONG_CASTLE, CAPTURE, FORWARD, BACKWARD
from engine.src.constants.types import MoveType
from engine.src.helpers.square_analysis import get_color, get_type
from engine.src.helpers.board_analysis import sight_on_square 
from engine.src.helpers.helpers import flip

class Generator():
    def __init__(self) -> None:
        self.move_manager: Moves = Moves()
        self.baseRatings: dict[str, int] = {CAPTURE: 3, FORWARD: 2, BACKWARD: 1}

    def get_moves(self, board: list[list[str]], kingPos: tuple[int, int]) -> list[MoveType]:
        '''given a board state return a list of possible moves
        
        this method returns a list of dictionaries that repersent moves. Each dict has the following key value pairs:
        \t old_x the original x of the piece
        \t old_y the original y of the piece
        \t new_x the new x of the piece
        \t new_y the new y of the piece
        \t the force of the move (used in ordering)

        Keyword arguments:
        \t board the board 
        \t kingPos the position of the king that is on the side you're looking for
        '''
        colorTarget = get_color(board[kingPos[1]][kingPos[0]])
        check = get_type(board[kingPos[1]][kingPos[0]])
        if (colorTarget != BLACK or colorTarget != WHITE) and check != KING:
            raise IndexError("Invalid king position")

        final: list[MoveType] = []
        for y, row in enumerate(board):
            for x, cell in enumerate(row):
                color = get_color(cell)
                if color == colorTarget:
                    moves = self.move_manager.get_all_moves(board, (x,y))
                    for move in moves:
                        if self.is_legal_move(board, kingPos, (x,y), (move[0],move[1])):
                            newMove: MoveType = MoveType(original=(x,y), new=(move[0],move[1]), rating=self.rate_move(board, move))
                            final.append(newMove)
        return final

    def rate_move(self, board: list[list[str]], move: tuple[int, int, str]) -> int:
        rating: int = self.baseRatings[move[2]]
        return rating


    def is_legal_move(self, board: list[list[str]], kingPos: tuple[int, int], oldPos: tuple[int, int], newPos: tuple[int, int], moveType='') -> bool:
        # simulate the new position
        new_board: list[list[str]] = board
        new_board[newPos[1]][newPos[0]] = new_board[oldPos[1]][oldPos[0]]
        new_board[oldPos[1]][oldPos[0]] = EMPTY

        # check if the king is in check
        enemyColor: str = flip(get_color(board[kingPos[1]][kingPos[0]]))
        sight: dict[str, list[tuple[int,int]]] = sight_on_square(new_board, kingPos)
        if len(sight[enemyColor]) > 0:
            return False
        
        # if the move is castling, we must also check that
        if moveType == SHORT_CASTLE:
            y: int = 0 if get_color(board[kingPos[1]][kingPos[0]]) == BLACK else 7
            for i in [(y,1), (y,2), (y,3)]:
                sight = sight_on_square(new_board, (y,1))
                if len(sight[enemyColor]) > 0:
                    return False
        elif moveType == LONG_CASTLE:
            y = 0 if get_color(board[kingPos[1]][kingPos[0]]) == BLACK else 7
            for i in [(y,6), (y,5)]:
                sight = sight_on_square(new_board, (y,1))
                if len(sight[enemyColor]) > 0:
                    return False
                
        return True
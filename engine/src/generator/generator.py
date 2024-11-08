from .moves import Moves
from engine.src.constants.static import BLACK, WHITE, KING, EMPTY, SHORT_CASTLE, LONG_CASTLE, CAPTURE, FORWARD, BACKWARD, DOUBLE_MOVE, LONG_CASTLE, SHORT_CASTLE, EN_PASSENT, PAWN, QUEEN, ROOK, BISHOP, KNIGHT
from engine.src.constants.engineTypes import MoveType
from engine.src.helpers.square_analysis import get_color, get_type
from engine.src.helpers.board_analysis import sight_on_square 
from engine.src.helpers.helpers import flip
from copy import deepcopy

class Generator():
    def __init__(self) -> None:
        '''
        Constructs a Generator, which is a class for generating legal moves
        '''
        self.move_manager: Moves = Moves()
        self.baseRatings: dict[str, int] = {CAPTURE: 3, FORWARD: 2, DOUBLE_MOVE: 2, BACKWARD: 1, LONG_CASTLE: 1, SHORT_CASTLE: 1}

    def get_moves(self, board: list[list[str]], kingPos: tuple[int, int]) -> list[MoveType]:
        '''given a board state return a list of possible moves
        
        this method returns a list of dictionaries that repersent moves. Each dict is a MoveType

        Keyword arguments:
        \t board the board 
        \t kingPos the position of the king that is on the side you're looking for
        '''
        colorTarget = get_color(board[kingPos[1]][kingPos[0]])
        check = get_type(board[kingPos[1]][kingPos[0]])
        if (colorTarget != BLACK and colorTarget != WHITE) or check != KING:
            raise IndexError("Invalid king position")

        final: list[MoveType] = []
        for y, row in enumerate(board):
            for x, cell in enumerate(row):
                color = get_color(cell)
                if color == colorTarget:             
                    moves = self.move_manager.get_all_moves(board, (x,y))
                    for move in moves:
                        if self.is_legal_move(board, kingPos, (x,y), move):
                            newMove: MoveType = MoveType(original=(x,y), new=(move[0],move[1]), rating=self.rate_move(board, move), promotion='')
                            promotion = get_type(cell) == PAWN and (move[1] == 0 or move[1] == 7)
                            if promotion:
                                for possiblity in [QUEEN, ROOK, BISHOP, KNIGHT]:
                                    mv: MoveType = deepcopy(newMove)
                                    mv['promotion'] = possiblity
                                    final.append(mv)
                            else:
                                final.append(newMove)

        final.sort(key=lambda x: x['rating'], reverse=True)
        return final

    def rate_move(self, board: list[list[str]], move: tuple[int, int, str]) -> int:
        '''Rates the move for ordering'''
        rating: int = self.baseRatings[move[2]]
        return rating


    def is_legal_move(self, board: list[list[str]], kingPos: tuple[int, int], oldPos: tuple[int, int], newPos: tuple[int, int, str], moveType='') -> bool:
        '''Returns if a move ends up putting the king in check.
        
        Keyword arguements:
        \t board - the board 
        \t kingPos - the position of the king which is on the side you're checking
        \t oldPos - the old position of the piece
        \t newPos - the new position of the piece
        \t moveType - used to indicate if a move is castling, as more squares must be checked there
        '''
        # simulate the new position
        new_board: list[list[str]] = deepcopy(board)
        new_board[newPos[1]][newPos[0]] = new_board[oldPos[1]][oldPos[0]]
        new_board[oldPos[1]][oldPos[0]] = EMPTY

        # if the move is enpassent, we must handle that
        color = get_color(board[kingPos[1]][kingPos[0]])
        if get_type(board[newPos[1]][newPos[0]]) == EN_PASSENT:
            offset = 1 if color == BLACK else -1
            new_board[newPos[1]+offset][newPos[0]] = EMPTY

        # if the king was moved, we need to update the king pos
        if get_type(board[oldPos[1]][oldPos[0]]) == KING:
            kingPos = (newPos[0], newPos[1])

        # check if the king is in check
        enemyColor: str = flip(color)
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
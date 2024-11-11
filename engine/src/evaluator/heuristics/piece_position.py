from engine.src.evaluator.heuristics.constants import piece_value_map
from engine.src.helpers.board_analysis import get_color, get_type, is_empty
from engine.src.constants.static import PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, KING, EMPTY, MIDDLE_GAME, END_GAME

def piece_position(square: str, location: tuple[int,int], is_endgame: bool) -> int:
    '''factor in the position of a piece'''
    factor: int = 1 if get_color(square) == WHITE else -1
    piece_type: str = get_type(square)

    if is_empty(square):
        return 0
    
    index = piece_type
    if piece_type == KING:
        index = END_GAME if is_endgame else MIDDLE_GAME

    x,y = location
    return piece_value_map[index][y][x] * factor
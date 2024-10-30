from engine.src.constants.constants import PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, KING
from engine.src.helpers.board_analysis import get_color, get_type, is_empty

pieceValue: dict[str, int] = {
    PAWN: 100,
    BISHOP: 300,
    KNIGHT: 300,
    ROOK: 500,
    QUEEN: 900,
}

def material_eval(board: list[list[str]]) -> int:
    '''Counts the material on each side'''
    val: int = 0
    for row in board:
        for square in row:
            if not is_empty(square)and get_type(square) != KING:
                factor: int = 1 if get_color(square) == WHITE else -1
                val += pieceValue[get_type(square)] * factor
    return val
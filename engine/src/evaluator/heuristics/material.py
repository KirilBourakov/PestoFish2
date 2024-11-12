from engine.src.constants.static import PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, KING, EMPTY
from engine.src.helpers.board_analysis import get_color, get_type, is_empty

pieceValue: dict[bool, dict[str, int]] = {
    True: {
        PAWN: 80,
        BISHOP: 325,
        KNIGHT: 310,
        ROOK: 465,
        QUEEN: 921,
    },
    False: {
        PAWN: 106,
        BISHOP: 264,
        KNIGHT: 248,
        ROOK: 478,
        QUEEN: 886,
    }
}

def material_eval(square: str, location: tuple[int,int], is_endgame: bool) -> int:
    '''Counts the material on each side'''
    factor: int = 1 if get_color(square) == WHITE else -1
    piece_type: str = get_type(square)
    if piece_type not in pieceValue[True]:
        return 0
    return pieceValue[is_endgame][piece_type] * factor
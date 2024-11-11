from engine.src.constants.static import PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, KING, EMPTY
from engine.src.helpers.board_analysis import get_color, get_type, is_empty

pieceValue: dict[bool, dict[str, int]] = {
    True: {
        PAWN: 84,
        BISHOP: 333,
        KNIGHT: 346,
        ROOK: 441,
        QUEEN: 921,
    },
    False: {
        PAWN: 106,
        BISHOP: 244,
        KNIGHT: 268,
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
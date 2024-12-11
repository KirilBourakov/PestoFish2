from engine.src.constants.engineTypes import boardType
from engine.src.generator.generator import Generator

from engine.src.constants.static import PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, KING, EMPTY, BLACK
from engine.src.helpers.square_analysis import get_type, get_color, is_empty

bad_squares: dict[str, dict[str, list[tuple[int,int]]]] = {
    BLACK: {
        PAWN: [],
        BISHOP: [(0,2), (0,5)],
        KNIGHT: [(0,1), (0,6)],
        ROOK: [(0,0), (0,7)],
        QUEEN: [],
        KING: [(0,4)],
    },
    WHITE: {
        PAWN: [],
        BISHOP: [(7,2), (7,5)],
        KNIGHT: [(7,1), (7,6)],
        ROOK: [(7,0), (7,7)],
        QUEEN: [],
        KING: [(7,4)],
    }
}

def development_eval(square: str, location: tuple[int,int], is_endgame: bool) -> int:
    if not is_empty(square):
        if location in bad_squares[get_color(square)][get_type(square)]:
            factor: int = -1 if get_color(square) == BLACK else 1
            return -20 * factor
    return 0
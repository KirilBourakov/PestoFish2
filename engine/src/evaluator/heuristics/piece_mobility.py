from engine.src.constants.engineTypes import boardType
from engine.src.generator.generator import Generator

from engine.src.constants.static import PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, KING, EMPTY, BLACK
from engine.src.helpers.board_analysis import find_king
from engine.src.helpers.square_analysis import get_type


pieceMobilityValue: dict[bool, dict[str, int]] = {
    True: {
        PAWN: 0,
        BISHOP: 3,
        KNIGHT: 3,
        ROOK: 3,
        QUEEN: 0,
        KING: 2,
    },
    False: {
        PAWN: 0,
        BISHOP: 3,
        KNIGHT: 5,
        ROOK: 3,
        QUEEN: 0,
        KING: 1,
    }
}

eng: Generator = Generator()
def piece_mobility(board: boardType, is_endgame: bool) -> int:
    value = 0
    for color in [BLACK, WHITE]:
        moves = eng.get_moves(board, find_king(board, color))
        factor = -1 if BLACK else 1
        for move in moves:
            square = board[move['original'][1]][move['original'][0]]
            value += factor * pieceMobilityValue[is_endgame][get_type(square)]
    return value
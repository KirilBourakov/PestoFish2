from enum import Enum

appsize = 512
grid_size = appsize / 8

resize_num = grid_size * .2


class Color(Enum):
    BLACK = 0
    WHITE = 1

class Piece(Enum):
    ROOK = 0
    BISHOP = 1
    KNIGHT = 2
    QUEEN = 3
    KING = 4
    PAWN = 5

# general flags
NORMAL_FLAG = "normal move"

# pawn flags
EN_PASSANT_FLAG = "en passent"
DOUBLE_MOVE_FLAG = "double move"
PROMOTION_FLAG = "promotion"

# king flags
SHORT_CASTLE_FLAG = "short castle"
LONG_CASTLE_FLAG = "long castle"

# game types
GAME_TYPE_PVP = "pvp"
GAME_TYPE_ENGINE_WHITE = "engine as white"
GAME_TYPE_ENGINE_BLACK = "engine as black"

#
CHECKMATE = 'checkmate'
STALEMATE = 'stalemate'

WHITE_TO_MOVE = 'White to Move'
BLACK_TO_MOVE = 'Black to Move'
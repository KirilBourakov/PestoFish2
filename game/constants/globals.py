appsize = 512
grid_size = appsize / 8

resize_num = grid_size * .2

PIECE_BLACK = "black"
PIECE_WHITE = "white"

PIECE_ROOK = "rook"
PIECE_BISHOP = "bishop"
PIECE_KNIGHT = "knight"
PIECE_KING = "king"
PIECE_QUEEN = "queen"
PIECE_PAWN = "pawn"

# general flags
NORMAL_FLAG = "normal move"

# pawn flags
EN_PASSENT_FLAG = "en passent"
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
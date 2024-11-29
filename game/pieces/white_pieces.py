from game.pieces.AbstractPiece import AbstractPiece
import game.assets.assets as assets
import game.constants.move_sets as move_sets
import game.constants.globals as globals

rookL = AbstractPiece(globals.PIECE_ROOK, assets.w_rook, move_sets.rook_moves, globals.PIECE_WHITE)
rookR = AbstractPiece(globals.PIECE_ROOK, assets.w_rook, move_sets.rook_moves, globals.PIECE_WHITE)
bishop = AbstractPiece(globals.PIECE_BISHOP, assets.w_bishop, move_sets.bishop_moves, globals.PIECE_WHITE)
knight = AbstractPiece(globals.PIECE_KNIGHT, assets.w_knight, move_sets.knight_moves, globals.PIECE_WHITE, hops=True)
king = AbstractPiece(globals.PIECE_KING, assets.w_king, move_sets.king_moves, globals.PIECE_WHITE)
queen = AbstractPiece(globals.PIECE_QUEEN, assets.w_queen, move_sets.queen_moves, globals.PIECE_WHITE)
pawn = AbstractPiece(globals.PIECE_PAWN, assets.w_pawn, move_sets.white_pawn_moves, globals.PIECE_WHITE)

# for engine
rook_unmoved = AbstractPiece(globals.PIECE_ROOK, assets.w_rook, move_sets.rook_moves, globals.PIECE_WHITE)
rook_moved = AbstractPiece(globals.PIECE_ROOK, assets.w_rook, move_sets.rook_moves, globals.PIECE_WHITE)
rook_moved.has_moved = True

king_unmoved = AbstractPiece(globals.PIECE_KING, assets.w_king, move_sets.king_moves, globals.PIECE_WHITE)
king_moved = AbstractPiece(globals.PIECE_KING, assets.w_king, move_sets.king_moves, globals.PIECE_WHITE)
king_moved.has_moved = True
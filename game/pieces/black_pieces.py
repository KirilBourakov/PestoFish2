from game.pieces.AbstractPiece import AbstractPiece
import game.assets.assets as assets
import game.constants.move_sets as move_sets
import game.constants.globals as globals

rookL = AbstractPiece(globals.PIECE_ROOK, assets.b_rook, move_sets.rook_moves, globals.PIECE_BLACK)
rookR = AbstractPiece(globals.PIECE_ROOK, assets.b_rook, move_sets.rook_moves, globals.PIECE_BLACK)
bishop = AbstractPiece(globals.PIECE_BISHOP, assets.b_bishop, move_sets.bishop_moves, globals.PIECE_BLACK)
knight = AbstractPiece(globals.PIECE_KNIGHT, assets.b_knight, move_sets.knight_moves, globals.PIECE_BLACK, hops=True)
king = AbstractPiece(globals.PIECE_KING, assets.b_king, move_sets.king_moves, globals.PIECE_BLACK)
queen = AbstractPiece(globals.PIECE_QUEEN, assets.b_queen, move_sets.queen_moves, globals.PIECE_BLACK)
pawn = AbstractPiece(globals.PIECE_PAWN, assets.b_pawn, move_sets.black_pawn_moves, globals.PIECE_BLACK)

# for engine
rook_unmoved = AbstractPiece(globals.PIECE_ROOK, assets.b_rook, move_sets.rook_moves, globals.PIECE_BLACK)
rook_moved = AbstractPiece(globals.PIECE_ROOK, assets.b_rook, move_sets.rook_moves, globals.PIECE_BLACK)
rook_moved.has_moved = True

king_unmoved = AbstractPiece(globals.PIECE_KING, assets.b_king, move_sets.king_moves, globals.PIECE_BLACK)
king_moved = AbstractPiece(globals.PIECE_KING, assets.b_king, move_sets.king_moves, globals.PIECE_BLACK)
king_moved.has_moved = True
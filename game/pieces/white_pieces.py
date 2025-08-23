from game.pieces.AbstractPiece import AbstractPiece
import game.assets.assets as assets
import game.constants.move_sets as move_sets
import game.constants.globals as globals

rookL = AbstractPiece(globals.Piece.ROOK, assets.w_rook, move_sets.rook_moves, globals.Color.WHITE)
rookR = AbstractPiece(globals.Piece.ROOK, assets.w_rook, move_sets.rook_moves, globals.Color.WHITE)
bishop = AbstractPiece(globals.Piece.BISHOP, assets.w_bishop, move_sets.bishop_moves, globals.Color.WHITE)
knight = AbstractPiece(globals.Piece.KNIGHT, assets.w_knight, move_sets.knight_moves, globals.Color.WHITE, hops=True)
king = AbstractPiece(globals.Piece.KING, assets.w_king, move_sets.king_moves, globals.Color.WHITE)
queen = AbstractPiece(globals.Piece.QUEEN, assets.w_queen, move_sets.queen_moves, globals.Color.WHITE)
pawn = AbstractPiece(globals.Piece.PAWN, assets.w_pawn, move_sets.white_pawn_moves, globals.Color.WHITE)

# for engine
rook_unmoved = AbstractPiece(globals.Piece.ROOK, assets.w_rook, move_sets.rook_moves, globals.Color.WHITE)
rook_moved = AbstractPiece(globals.Piece.ROOK, assets.w_rook, move_sets.rook_moves, globals.Color.WHITE)
rook_moved.has_moved = True

king_unmoved = AbstractPiece(globals.Piece.KING, assets.w_king, move_sets.king_moves, globals.Color.WHITE)
king_moved = AbstractPiece(globals.Piece.KING, assets.w_king, move_sets.king_moves, globals.Color.WHITE)
king_moved.has_moved = True
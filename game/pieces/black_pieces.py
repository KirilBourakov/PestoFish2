from game.pieces.AbstractPiece import AbstractPiece
import game.assets.assets as assets
import game.constants.move_sets as move_sets
import game.constants.globals as globals

rookL = AbstractPiece(globals.Piece.ROOK, assets.b_rook, move_sets.rook_moves, globals.Color.BLACK)
rookR = AbstractPiece(globals.Piece.ROOK, assets.b_rook, move_sets.rook_moves, globals.Color.BLACK)
bishop = AbstractPiece(globals.Piece.BISHOP, assets.b_bishop, move_sets.bishop_moves, globals.Color.BLACK)
knight = AbstractPiece(globals.Piece.KNIGHT, assets.b_knight, move_sets.knight_moves, globals.Color.BLACK, hops=True)
king = AbstractPiece(globals.Piece.KING, assets.b_king, move_sets.king_moves, globals.Color.BLACK)
queen = AbstractPiece(globals.Piece.QUEEN, assets.b_queen, move_sets.queen_moves, globals.Color.BLACK)
pawn = AbstractPiece(globals.Piece.PAWN, assets.b_pawn, move_sets.black_pawn_moves, globals.Color.BLACK)

# for engine
rook_unmoved = AbstractPiece(globals.Piece.ROOK, assets.b_rook, move_sets.rook_moves, globals.Color.BLACK)
rook_moved = AbstractPiece(globals.Piece.ROOK, assets.b_rook, move_sets.rook_moves, globals.Color.BLACK)
rook_moved.has_moved = True

king_unmoved = AbstractPiece(globals.Piece.KING, assets.b_king, move_sets.king_moves, globals.Color.BLACK)
king_moved = AbstractPiece(globals.Piece.KING, assets.b_king, move_sets.king_moves, globals.Color.BLACK)
king_moved.has_moved = True
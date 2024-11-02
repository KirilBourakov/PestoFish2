from game.pieces.abstract_piece import Abstract_Piece
import game.assets.assets as assets
import game.constants.move_sets as move_sets
import game.constants.globals as globals

rookL = Abstract_Piece(globals.PIECE_ROOK, assets.w_rook, move_sets.rook_moves, globals.PIECE_WHITE)
rookR = Abstract_Piece(globals.PIECE_ROOK, assets.w_rook, move_sets.rook_moves, globals.PIECE_WHITE)
bishop = Abstract_Piece(globals.PIECE_BISHOP, assets.w_bishop, move_sets.bishop_moves, globals.PIECE_WHITE)
knight = Abstract_Piece(globals.PIECE_KNIGHT, assets.w_knight, move_sets.knight_moves, globals.PIECE_WHITE, hops=True)
king = Abstract_Piece(globals.PIECE_KING, assets.w_king, move_sets.king_moves, globals.PIECE_WHITE)
queen = Abstract_Piece(globals.PIECE_QUEEN, assets.w_queen, move_sets.queen_moves, globals.PIECE_WHITE)
pawn = Abstract_Piece(globals.PIECE_PAWN, assets.w_pawn, move_sets.white_pawn_moves, globals.PIECE_WHITE)

# for engine
rook_unmoved = Abstract_Piece(globals.PIECE_ROOK, assets.w_rook, move_sets.rook_moves, globals.PIECE_WHITE)
rook_moved = Abstract_Piece(globals.PIECE_ROOK, assets.w_rook, move_sets.rook_moves, globals.PIECE_WHITE)
rook_moved.has_moved = False

king_unmoved = Abstract_Piece(globals.PIECE_KING, assets.w_king, move_sets.king_moves, globals.PIECE_WHITE)
king_moved = Abstract_Piece(globals.PIECE_KING, assets.w_king, move_sets.king_moves, globals.PIECE_WHITE)
king_moved.has_moved = False
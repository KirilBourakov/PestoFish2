from game.pieces.abstract_piece import Abstract_Piece
import game.assets.assets as assets
import game.constants.move_sets as move_sets
import game.constants.globals as globals

rookL = Abstract_Piece(globals.PIECE_ROOK, assets.b_rook, move_sets.rook_moves, globals.PIECE_BLACK)
rookR = Abstract_Piece(globals.PIECE_ROOK, assets.b_rook, move_sets.rook_moves, globals.PIECE_BLACK)
bishop = Abstract_Piece(globals.PIECE_BISHOP, assets.b_bishop, move_sets.bishop_moves, globals.PIECE_BLACK)
knight = Abstract_Piece(globals.PIECE_KNIGHT, assets.b_knight, move_sets.knight_moves, globals.PIECE_BLACK, hops=True)
king = Abstract_Piece(globals.PIECE_KING, assets.b_king, move_sets.king_moves, globals.PIECE_BLACK)
queen = Abstract_Piece(globals.PIECE_QUEEN, assets.b_queen, move_sets.queen_moves, globals.PIECE_BLACK)
pawn = Abstract_Piece(globals.PIECE_PAWN, assets.b_pawn, move_sets.black_pawn_moves, globals.PIECE_BLACK)

# for engine
rook_unmoved = Abstract_Piece(globals.PIECE_ROOK, assets.b_rook, move_sets.rook_moves, globals.PIECE_BLACK)
rook_moved = Abstract_Piece(globals.PIECE_ROOK, assets.b_rook, move_sets.rook_moves, globals.PIECE_BLACK)
rook_moved.has_moved = True

king_unmoved = Abstract_Piece(globals.PIECE_KING, assets.b_king, move_sets.king_moves, globals.PIECE_BLACK)
king_moved = Abstract_Piece(globals.PIECE_KING, assets.b_king, move_sets.king_moves, globals.PIECE_BLACK)
king_moved.has_moved = True
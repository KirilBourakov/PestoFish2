from pieces.abstract_piece import Abstract_Piece
import assets.assets as assets
import pieces.move_sets as move_sets
import globals

rookL = Abstract_Piece(globals.PIECE_ROOK, assets.w_rookL, move_sets.rook_moves, globals.PIECE_WHITE)
rookR = Abstract_Piece(globals.PIECE_ROOK, assets.w_rookR, move_sets.rook_moves, globals.PIECE_WHITE)
bishop = Abstract_Piece(globals.PIECE_BISHOP, assets.w_bishop, move_sets.bishop_moves, globals.PIECE_WHITE)
knight = Abstract_Piece(globals.PIECE_KNIGHT, assets.w_knight, move_sets.knight_moves, globals.PIECE_WHITE, hops=True)
king = Abstract_Piece(globals.PIECE_KING, assets.w_king, move_sets.king_moves, globals.PIECE_WHITE)
queen = Abstract_Piece(globals.PIECE_QUEEN, assets.w_queen, move_sets.queen_moves, globals.PIECE_WHITE)
pawn = Abstract_Piece(globals.PIECE_PAWN, assets.w_pawn, move_sets.white_pawn_moves, globals.PIECE_WHITE)
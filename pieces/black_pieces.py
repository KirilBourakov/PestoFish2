from pieces.abstract_piece import Abstract_Piece
import assets.assets as assets
import pieces.move_sets as move_sets
import globals

rookL = Abstract_Piece("rook", assets.b_rookL, move_sets.rook_moves, globals.PIECE_BLACK)
rookR = Abstract_Piece("rook", assets.b_rookR, move_sets.rook_moves, globals.PIECE_BLACK)
bishop = Abstract_Piece("bishop", assets.b_bishop, move_sets.bishop_moves, globals.PIECE_BLACK)
knight = Abstract_Piece("knight", assets.b_knight, move_sets.knight_moves, globals.PIECE_BLACK, hops=True)
king = Abstract_Piece("king", assets.b_king, move_sets.king_moves, globals.PIECE_BLACK)
queen = Abstract_Piece("queen", assets.b_queen, move_sets.queen_moves, globals.PIECE_BLACK)
pawn = Abstract_Piece("pawn", assets.b_pawn, move_sets.black_pawn_moves, globals.PIECE_BLACK)
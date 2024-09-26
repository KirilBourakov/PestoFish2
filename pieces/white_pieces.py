from pieces.abstract_piece import Abstract_Piece
import assets.assets as assets
import pieces.move_sets as move_sets

rookL = Abstract_Piece("rook", assets.w_rookL, move_sets.rook_moves, "white")
rookR = Abstract_Piece("rook", assets.w_rookR, move_sets.rook_moves, "white")
bishop = Abstract_Piece("bishop", assets.w_bishop, move_sets.bishop_moves, "white")
knight = Abstract_Piece("knight", assets.w_knight, move_sets.knight_moves, "white", hops=True)
king = Abstract_Piece("king", assets.w_king, move_sets.king_moves, "white")
queen = Abstract_Piece("queen", assets.w_queen, move_sets.queen_moves, "white")
pawn = Abstract_Piece("pawn", assets.w_pawn, move_sets.white_pawn_moves, "white")
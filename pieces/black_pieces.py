from pieces.abstract_piece import Abstract_Piece
import assets.assets as assets
import pieces.move_sets as move_sets

rookL = Abstract_Piece("rook", assets.b_rookL, move_sets.rook_moves, "black")
rookR = Abstract_Piece("rook", assets.b_rookR, move_sets.rook_moves, "black")
bishop = Abstract_Piece("bishop", assets.b_bishop, move_sets.bishop_moves, "black")
knight = Abstract_Piece("knight", assets.b_knight, move_sets.knight_moves, "black", hops=True)
king = Abstract_Piece("king", assets.b_king, move_sets.king_moves, "black")
queen = Abstract_Piece("queen", assets.b_queen, move_sets.queen_moves, "black")
pawn = Abstract_Piece("pawn", assets.b_pawn, move_sets.black_pawn_moves, "black")
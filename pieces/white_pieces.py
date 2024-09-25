from pieces.abstract_piece import Abstract_Piece
import assets.assets as assets

rookL = Abstract_Piece(assets.w_rookL, [], "white")
rookR = Abstract_Piece(assets.w_rookR, [], "white")
bishop = Abstract_Piece(assets.w_bishop, [], "white")
knight = Abstract_Piece(assets.w_knight, [], "white", hops=True)
king = Abstract_Piece(assets.w_king, [], "white")
queen = Abstract_Piece(assets.w_queen, [], "white")
pawn = Abstract_Piece(assets.w_pawn, [], "white")
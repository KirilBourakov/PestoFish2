from pieces.abstract_piece import Abstract_Piece
import assets.assets as assets

rookL = Abstract_Piece(assets.b_rookL, [], "black")
rookR = Abstract_Piece(assets.b_rookR, [], "black")
bishop = Abstract_Piece(assets.b_bishop, [], "black")
knight = Abstract_Piece(assets.b_knight, [], "black", hops=True)
king = Abstract_Piece(assets.b_king, [], "black")
queen = Abstract_Piece(assets.b_queen, [], "black")
pawn = Abstract_Piece(assets.b_pawn, [], "black")
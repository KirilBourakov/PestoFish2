from pieces.abstract_piece import Abstract_Piece
import assets.assets as assets

rookL = Abstract_Piece(assets.b_rookL, [])
rookR = Abstract_Piece(assets.b_rookR, [])
bishop = Abstract_Piece(assets.b_bishop, [])
knight = Abstract_Piece(assets.b_knight, [], hops=True)
king = Abstract_Piece(assets.b_king, [])
queen = Abstract_Piece(assets.b_queen, [])
pawn = Abstract_Piece(assets.b_pawn, [])
import tkinter

app = tkinter.Tk()
size = (app.winfo_screenwidth(), app.winfo_screenheight())

appsize = size[1] / 2
grid_size = appsize / 8

resize_num = grid_size * .2

PIECE_BLACK = "black"
PIECE_WHITE = "white"

PIECE_ROOK = "rook"
PIECE_BISHOP = "bishop"
PIECE_KNIGHT = "knight"
PIECE_KING = "king"
PIECE_QUEEN = "queen"
PIECE_PAWN = "pawn"

# general flags
NORMAL_FLAG = "normal move"

# pawn flags
EN_PASSENT_FLAG = "en passent"
DOUBLE_MOVE_FLAG = "double move"

# king flags
SHORT_CASTLE_FLAG = "short castle"
LONG_CASTLE_FLAG = "long castle"
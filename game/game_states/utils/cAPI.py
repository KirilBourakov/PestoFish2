from pygame import Surface

import cEngine
from game.assets import assets


class cAPI:
    engine = cEngine.Engine()

    @staticmethod
    def get_surface(piece: cEngine.Piece) -> Surface | None:
        match piece:
            # ---- WHITE PIECES ----
            case cEngine.Piece.WHITE_PAWN:
                return assets.w_pawn
            case cEngine.Piece.WHITE_BISHOP:
                return assets.w_bishop
            case cEngine.Piece.WHITE_KNIGHT:
                return assets.w_knight
            case cEngine.Piece.WHITE_ROOK:
                return assets.w_rook
            case cEngine.Piece.WHITE_QUEEN:
                return assets.w_queen
            case cEngine.Piece.WHITE_KING:
                return assets.w_king
            # ---- BLACK PIECES ----
            case cEngine.Piece.BLACK_PAWN:
                return assets.b_pawn
            case cEngine.Piece.BLACK_BISHOP:
                return assets.b_bishop
            case cEngine.Piece.BLACK_KNIGHT:
                return assets.b_knight
            case cEngine.Piece.BLACK_ROOK:
                return assets.b_rook
            case cEngine.Piece.BLACK_QUEEN:
                return assets.b_queen
            case cEngine.Piece.BLACK_KING:
                return assets.b_king

            # ---- EMPTY ----
            case cEngine.Piece.EMPTY:
                return None

            case _:
                raise RuntimeError("Unknown piece {}".format(piece))
from pygame import Surface

import cEngine
from game.assets import assets


class cAPI:
    engine = cEngine.Engine()

    @staticmethod
    def clear():
        cAPI.engine = cEngine.Engine()

    @staticmethod
    def get_curr_board() -> list:
        return cAPI.engine.getState().getBoard()

    @staticmethod
    def active_color() -> cEngine.Color:
        return cAPI.engine.getState().get_active_color()

    @staticmethod
    def same_color(color: cEngine.Color, piece: cEngine.Piece) -> bool:
        return cEngine.same_color(color, piece)

    @staticmethod
    def getAt(gridx: int, gridy: int) -> cEngine.Piece:
        return cAPI.engine.getState().getBoard()[gridy][gridx]

    @staticmethod
    def is_legal_move(start: tuple[int, int], end: tuple[int, int]) -> bool:
        start_pos = cEngine.BoardPosition(*start)
        end_pos = cEngine.BoardPosition(*end)
        return cAPI.engine.getState().is_legal_move(start_pos, end_pos)

    @staticmethod
    def make_move(start: tuple[int, int], end: tuple[int, int]):
        start_pos = cEngine.BoardPosition(*start)
        end_pos = cEngine.BoardPosition(*end)
        return cAPI.engine.getState().translate_and_move(start_pos, end_pos)

    @staticmethod
    def get_half_move_clock():
        return cAPI.engine.getState().get_half_move_clock()

    @staticmethod
    def game_in_play():
        return cAPI.engine.getState().get_game_state() == cEngine.GameState.IN_PLAY

    @staticmethod
    def get_state_message():
        state = cAPI.engine.getState().get_game_state()
        match state:
            case cEngine.GameState.WHITE_WIN:
                return 'White has won'
            case cEngine.GameState.BLACK_WIN:
                return 'Black has won'
            case cEngine.GameState.STALEMATE:
                return 'Stalemate'
            case cEngine.GameState.DRAW:
                return 'Draw'
            case _:
                raise RuntimeError("get_state_message() not supported for state {}".format(state))

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
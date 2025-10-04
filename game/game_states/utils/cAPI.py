from typing import Optional

from pygame import Surface

import cEngine
from game.assets import assets
from game.constants.globals import Piece

PIECE = cEngine.Piece

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
    def same_color(color: cEngine.Color, piece: PIECE) -> bool:
        return cEngine.same_color(color, piece)

    @staticmethod
    def getAt(gridx: int, gridy: int) -> PIECE:
        return cAPI.engine.getState().getBoard()[gridy][gridx]

    @staticmethod
    def is_legal_move(start: tuple[int, int], end: tuple[int, int]) -> bool:
        start_pos = cEngine.BoardPosition(*start)
        end_pos = cEngine.BoardPosition(*end)
        return cAPI.engine.getState().is_legal_move(start_pos, end_pos)

    @staticmethod
    def make_move(start: tuple[int, int], end: tuple[int, int], promotion: Optional[PIECE] = None):
        start_pos = cEngine.BoardPosition(*start)
        end_pos = cEngine.BoardPosition(*end)
        cAPI.engine.getState().translate_and_move(start_pos, end_pos, promotion)

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
    def get_surface(piece: PIECE) -> Surface | None:
        match piece:
            # ---- WHITE PIECES ----
            case PIECE.WHITE_PAWN:
                return assets.w_pawn
            case PIECE.WHITE_BISHOP:
                return assets.w_bishop
            case PIECE.WHITE_KNIGHT:
                return assets.w_knight
            case PIECE.WHITE_ROOK:
                return assets.w_rook
            case PIECE.WHITE_QUEEN:
                return assets.w_queen
            case PIECE.WHITE_KING:
                return assets.w_king
            # ---- BLACK PIECES ----
            case PIECE.BLACK_PAWN:
                return assets.b_pawn
            case PIECE.BLACK_BISHOP:
                return assets.b_bishop
            case PIECE.BLACK_KNIGHT:
                return assets.b_knight
            case PIECE.BLACK_ROOK:
                return assets.b_rook
            case PIECE.BLACK_QUEEN:
                return assets.b_queen
            case PIECE.BLACK_KING:
                return assets.b_king

            # ---- EMPTY ----
            case PIECE.EMPTY:
                return None

            case _:
                raise RuntimeError("Unknown piece {}".format(piece))
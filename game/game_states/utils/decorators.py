import game.constants.globals as globals
from game.game_states.utils.cAPI import cAPI, COLOR


def disable_on_engine_turn(func):
    def wrapper(self, *args, **kwargs):
        black_engine_turn = (
            self.game_type == globals.GAME_TYPE_ENGINE_BLACK
            and cAPI.active_color == COLOR.BLACK
        )
        white_engine_turn = (
            self.game_type == globals.GAME_TYPE_ENGINE_WHITE
            and cAPI.active_color == COLOR.WHITE
        )
        if not black_engine_turn and not white_engine_turn:
            func(self, *args, **kwargs)
        return

    return wrapper


def run_engine(func):
    def wrapper(self, *args, **kwargs):
        func(self, *args, **kwargs)

        black_engine_turn = (
            self.game_type == globals.GAME_TYPE_ENGINE_BLACK
            and cAPI.active_color() == COLOR.BLACK
        )
        white_engine_turn = (
            self.game_type == globals.GAME_TYPE_ENGINE_WHITE
            and cAPI.active_color() == COLOR.WHITE
        )
        if black_engine_turn or white_engine_turn:
            cAPI.make_engine_move()
        return

    return wrapper

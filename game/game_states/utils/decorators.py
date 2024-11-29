import game.constants.globals as globals
from .EngineAPI import EngineAPI

def disable_on_engine_turn(func):
    def wrapper(self, *args, **kwargs):
        black_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_BLACK and self.move_counter % 2 != 0
        white_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_WHITE and self.move_counter % 2 == 0
        if not black_engine_turn and not white_engine_turn:
            func(self, *args, **kwargs)
        return 
    return wrapper

def run_engine(func):
    def wrapper(self, *args, **kwargs):
        func(self, *args, **kwargs)
        black_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_BLACK and self.move_counter % 2 != 0 and self.promotion == None
        white_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_WHITE and self.move_counter % 2 == 0 and self.promotion == None
        if black_engine_turn or white_engine_turn:
            EngineAPI.engine_make_move(self)
        return  
    return wrapper
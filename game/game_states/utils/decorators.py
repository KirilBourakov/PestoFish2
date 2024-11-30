import game.constants.globals as globals
from threading import Thread
from .EngineAPI import EngineAPI
import pygame

def disable_on_engine_turn(func):
    def wrapper(self, *args, **kwargs):
        black_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_BLACK and self.move_counter % 2 != 0 and self.promotion is None
        white_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_WHITE and self.move_counter % 2 == 0 and self.promotion is None
        if not black_engine_turn and not white_engine_turn:
            func(self, *args, **kwargs)
        return 
    return wrapper

def run_engine(func):
    def wrapper(self, *args, **kwargs):
        func(self, *args, **kwargs)
        
        black_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_BLACK and self.move_counter % 2 != 0 and self.promotion is None
        white_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_WHITE and self.move_counter % 2 == 0 and self.promotion is None
        if black_engine_turn or white_engine_turn:
            EngineAPI.engine_make_move(self)    
        return  
    return wrapper

import game.constants.globals as globals

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
        # TODO: the enggine should take direct control of the board. This will make the logic simpler, and incorrect move gen in the engine
        # easier to spot
        black_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_BLACK and self.move_counter % 2 != 0
        white_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_WHITE and self.move_counter % 2 == 0
        if black_engine_turn or white_engine_turn:
            self.engine.accept_board(self.convert_for_engine())
            best_move = self.engine.get_best_move()
            self.make_legal_move(best_move['new'], piece_location=best_move['original'])
        return  
    return wrapper

import game.constants.globals as globals
import game.pieces.black_pieces as black
import game.pieces.white_pieces as white
import game.pieces.en_passent as en_passent

# TODO: this approuch may create bugs around piece premotions
piece_dict = {
    'BK': black.king_unmoved,
    'bk': black.king_moved,
    'bk': black.knight,
    'bb': black.bishop,
    'bp': black.pawn,
    'br': black.rook_moved,
    'Br': black.rook_unmoved,
    'bq': black.queen,

    'WK': white.king_unmoved,
    'wK': white.king_moved,
    'wk': white.knight,
    'wb': white.bishop,
    'wp': white.pawn,
    'wr': white.rook_moved,
    'Wr': white.rook_unmoved,
    'wq': white.queen
}

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
        # TODO: the engine should take direct control of the board. This will make the logic simpler, and incorrect move gen in the engine
        # easier to spot. Maybe have the board updated to what the engine sees
        # engine move not being accepted leads to infinite loop
        black_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_BLACK and self.move_counter % 2 != 0
        white_engine_turn = self.game_type == globals.GAME_TYPE_ENGINE_WHITE and self.move_counter % 2 == 0
        if black_engine_turn or white_engine_turn:
            self.engine.accept_board(self.convert_for_engine())
            best_move = self.engine.get_best_move()
            new_pos = self.engine.result(self.engine.board, best_move)
            translated_board = []
            for i in range(8):
                translated_board.append([None]*8)

            self.move_counter += 1
            move_is_pawn = self.engine.board[best_move['original'][1]][best_move['original'][0]][1] == 'p'
            move_is_capture = len(self.engine.board[best_move['new'][1]][best_move['new'][0]]) != 0
            if not move_is_pawn or not move_is_capture:
                self.fifty_move_rule_counter += 1

            for y, row in enumerate(new_pos):
                for x, square in enumerate(row):
                    if square in piece_dict:
                        translated_board[y][x] = piece_dict[square]
                    elif square == 'ep':
                        color = globals.PIECE_BLACK if y == 1 else globals.PIECE_WHITE
                        offset = -1 if color == globals.PIECE_BLACK else 1
                        translated_board[y][x] = en_passent(self.board.move_counter, color, y+offset)
            self.board = translated_board           

        return  
    return wrapper
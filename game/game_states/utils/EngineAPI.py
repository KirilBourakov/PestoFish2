import game.constants.globals as globals
import game.pieces.black_pieces as black
import game.pieces.white_pieces as white
import game.pieces.EnPassent as EnPassent
from engine.src.Engine import Engine

class EngineAPI():
    # TODO: this approuch may create bugs around piece premotions
    piece_dict = {
        'BK': black.king_unmoved,
        'bK': black.king_moved,
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
    engine = Engine()

    @classmethod
    def engine_make_move(self, playState):
        self.engine.accept_board(playState.convert_for_engine())
        best_move = self.engine.get_best_move()
        new_pos = self.engine.result(self.engine.board, best_move)
        print("----------------- new Pos ----------")
        for row in new_pos:
            print(row)
        translated_board = []
        for i in range(8):
            translated_board.append([None]*8)

        playState.move_counter += 1
        move_is_pawn = self.engine.board[best_move['original'][1]][best_move['original'][0]][1] == 'p'
        move_is_capture = len(self.engine.board[best_move['new'][1]][best_move['new'][0]]) != 0
        if not move_is_pawn or not move_is_capture:
            playState.fifty_move_rule_counter += 1
        
        for y, row in enumerate(new_pos):
            for x, square in enumerate(row):
                if square in self.piece_dict:
                    translated_board[y][x] = self.piece_dict[square]
                elif square == 'ep':
                    color = globals.PIECE_BLACK if y == 1 else globals.PIECE_WHITE
                    offset = -1 if color == globals.PIECE_BLACK else 1
                    translated_board[y][x] = EnPassent(playState.board.move_counter, color, y+offset)
        playState.board = translated_board
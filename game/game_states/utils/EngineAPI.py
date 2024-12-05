import game.constants.globals as globals
import game.assets.assets as assets
import game.pieces.black_pieces as black
import game.pieces.white_pieces as white
from game.pieces.EnPassent import EnPassent
from engine.src.Engine import Engine
import pygame

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
    def convert_for_engine(self, gameObj):
        '''returns a string representing all relevent board state. \n
        string takes the form of the 8 rows devided by / then the number of half moves, then the number of half moves for the fifty move rule.
        For example:
        
        Br bk bb bq BK bb bk Br /bp bp bp bp -- bp bp bp /-- -- -- -- be -- -- -- /-- -- -- -- bp -- -- -- /-- -- -- -- wp -- -- -- /-- -- -- -- -- -- -- -- /wp wp wp wp -- wp wp wp /Wr wk wb wq WK wb wk Wr /2/0

        is the position after 1.e4 e5. 

        Pieces are represented using the first letter of their color, and the first letter of their name, both lowercased, but:
        \t The king has his k uppercased to differentiate from the knight
        \t The rook and king may an uppercase color letter, if they have not moved
        \t enpassent is represented as a piece with a second letter of e, and only appears if enpassent is a valid move
        '''
        strState = ''

        for row in gameObj.board:
            strRow = ''
            for square in row:
                strSquare = "-- "
                # if square has a piece
                if square is not None and square.type != globals.EN_PASSENT_FLAG:
                    # king represented as wK/bK if moved or WK/BK if not
                    if square.type == globals.PIECE_KING:
                        strSquare = (square.color[0].lower() if square.has_moved else square.color[0].upper()) + square.type[0].upper() + " "
                    # rook repersented by wr/br if moved else Wr/Br
                    elif square.type == globals.PIECE_ROOK:
                        strSquare = (square.color[0].lower() if square.has_moved else square.color[0].upper()) + square.type[0].lower() + " "
                    # other pieces represented by w[first letter]/b[first letter]
                    else:
                        strSquare = square.color[0].lower() + square.type[0].lower() + " "

                # if square is enpassent, represented by we/be
                if square is not None and square.type == globals.EN_PASSENT_FLAG:
                    if square.turn_num == gameObj.move_counter:
                        strSquare = square.color[0].lower() + "e" + " "
                strRow += strSquare
            strState += strRow + "/"
        
        strState += f"{gameObj.move_counter}/{gameObj.fifty_move_rule_counter}"
        return strState.strip()
    
    @classmethod
    def engine_make_move(self, playState):
        pygame.display.update()

        self.engine.accept_board(self.convert_for_engine(playState))
        best_move = self.engine.get_best_move()

        # prevent enpassent flags from becoming zombies
        for y, row in enumerate(self.engine.board):
            for x, square in enumerate(row):
                if square == 'be' or square == 'we':
                    self.engine.board[y][x] = '  '
                    
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
                elif square == 'be' or square == 'we':
                    color = globals.PIECE_BLACK if y == 1 else globals.PIECE_WHITE
                    offset = -1 if color == globals.PIECE_BLACK else 1
                    translated_board[y][x] = EnPassent(playState.move_counter, color, y+offset)
        assets.sfx_takes.play()
        playState.board = translated_board       
        playState.update_bottom_text()
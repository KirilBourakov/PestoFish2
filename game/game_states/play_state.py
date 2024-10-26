import pygame, copy
import game.assets.assets as assets
import game.pieces.white_pieces as wp
import game.pieces.black_pieces as bp
import game.pieces.en_passent as ep
import game.constants.globals as globals
import game.constants.move_sets as mv
from game.game_states.promotion import Promotion
from game.game_states.abstract_state import Abstract_State
from game.game_states.decorators import disable_on_engine_turn, run_engine
from engine.engine import get_move_and_play

# for testing
from engine.src.engine import engine
from engine.src.generator.moves import Moves

class Play_State(Abstract_State):
    def __init__(self, board=None):
        '''The constructor.
        
        Keyword arguments:
        board -- takes in a board position, and returns a board with a shallow copy of it (default=None)
        '''
        self.enter()

        if board is not None:
            for i, col in enumerate(board):
                self.board[i] = copy.copy(col)

    def enter(self, *args):
        '''Initialize all the instance variables to a starting state

        Keyword arguments:
        args -- a list of lists that contains the game type at index 0,0
        '''
        self.board = [
            [bp.rookL, bp.knight, bp.bishop, bp.queen, bp.king, bp.bishop, bp.knight, bp.rookR],
            [bp.pawn] * 8,
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [wp.pawn] * 8,
            [wp.rookL, wp.knight, wp.bishop, wp.queen, wp.king, wp.bishop, wp.knight, wp.rookR]
        ]
        self.move_counter = 0
        self.fifty_move_rule_counter = 0
        self.past_board_states = {
            str(self.board): 1
        }
        self.selected_square = None
        
        self.promotion = None
        self.game_over = None

        self.game_type = globals.GAME_TYPE_PVP if len(args) == 0 else args[0][0]
        self.engine = get_move_and_play

    def convert_for_engine(self):
        '''returns a string representing all relevent board state. \n
        string takes the form of the 8 rows devided by / then the number of half moves, then the number of half moves for the fifty move rule.
        For example:
        
        Br bk bb bq BK bb bk Br /bp bp bp bp -- bp bp bp /-- -- -- -- be -- -- -- /-- -- -- -- bp -- -- -- /-- -- -- -- wp -- -- -- /-- -- -- -- -- -- -- -- /wp wp wp wp -- wp wp wp /Wr wk wb wq WK wb wk Wr /2/0

        is the position after 1.e4 e5. 

        Pieces are represented using the first letter of their color, and the first letter of their name, both lowercased, but:
        \t The king has his k uppercased to differentiate from the knight
        \t The rook and king may an uppercase color letter, if they have not moved
        \t enpassent is represented as a piece with a first letter of e, and only appears if enpassent is a valid move
        '''
        strState = ''

        for row in self.board:
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
                    if square.turn_num == self.move_counter:
                        strSquare = square.color[0].lower() + "e" + " "
                strRow += strSquare
            strState += strRow + "/"
        
        strState += f"{self.move_counter}/{self.fifty_move_rule_counter}"
        return strState.strip()

    @disable_on_engine_turn
    def handle_click(self, gridx, gridy):
        '''Handles user clicks. Disabled when it's the engine's turn.
        
        Keyword arguments:
        gridx -- the x position of the click location on the board
        gridy -- the y position of the click location on the board
        '''
        e = engine()
        r = Moves()
        r.get_complex_moves(e.accept_board(self.convert_for_engine()), (gridx, gridy))
        r.get_simple_moves(e.accept_board(self.convert_for_engine()), (gridx, gridy))
        
        if self.promotion is not None:
            self.promotion.handle_click((gridx, gridy), self)
            return

        if self.selected_square == (gridx, gridy):
            return
        if self.selected_square is None and (self.board[gridy][gridx] is not None and self.board[gridy][gridx].type != globals.EN_PASSENT_FLAG):
            if self.move_counter % 2 == 0 and self.board[gridy][gridx].color == globals.PIECE_WHITE: 
                self.selected_square = (gridx, gridy)
            elif self.move_counter % 2 != 0 and self.board[gridy][gridx].color == globals.PIECE_BLACK:
                self.selected_square = (gridx, gridy) 
            return
        
        if self.selected_square != None:
            self.make_legal_move((gridx, gridy))
            self.selected_square = None
            return
        
    def get_king_pos(self, color):
        '''Returns the kings position
        
        Keyword arguments:
        color -- the color of the king you're looking for
        '''
        for y, colomn in enumerate(self.board):
            for x, square in enumerate(colomn):
                if square is not None and square.color == color and square.type == globals.PIECE_KING:
                    return (x,y)
        raise Exception("King not found.") 
    
    def get_turn(self):
        '''Returns which colors turn it is'''
        if self.move_counter % 2 == 0:
            return globals.PIECE_WHITE
        return globals.PIECE_BLACK
    
    def inbound(self, pos):
        '''returns if a position is within the bounds of the board
        
        Keyword arguments:
        pos -- tuple that represents the position
        '''
        x,y = pos
        return (x >= 0) and (y >= 0) and (x < 8) and (y < 8)
        
    def get_sight_on_square(self, square):
        '''returns a list of all the pieces that attack a square
        
        Keyword arguments:
        square -- the square
        '''
        x,y = square
        found = []
        
        # gets any rooks or queens looking at the square
        rook_directions = [(0,1), (0,-1), (1,0), (-1,0)]
        for direction in rook_directions:
            f = self.search(square, direction, [globals.PIECE_ROOK, globals.PIECE_QUEEN])
            if f is not None:
                found.append(f)

        # gets any bishops or queens looking at the square
        bishop_directions = [(1,1), (-1,-1), (-1,1), (1,-1)]
        for direction in bishop_directions:
            f = self.search(square, direction, [globals.PIECE_BISHOP, globals.PIECE_QUEEN])
            if f is not None:
                found.append(f)
        
        # gets any knights looking at the square
        for move in mv.knight_moves:
            newx, newy = x + move[0], y + move[1]
            if self.inbound((newx, newy)):
                if self.board[newy][newx] is not None and self.board[newy][newx].type == globals.PIECE_KNIGHT:
                    found.append((newx,newy))

        # gets any pawn or kings looking at the square
        # white pawn
        for move in [(1,-1), (-1,-1)]:
            newx, newy = x + move[0], y + move[1]
            if self.inbound((newx, newy)):
                occupied_by_king = self.board[newy][newx] is not None and self.board[newy][newx].type == globals.PIECE_KING
                if occupied_by_king:
                    found.append((newx,newy))
                elif self.board[newy][newx] is not None and self.board[newy][newx].type == globals.PIECE_PAWN:
                    found.append((newx,newy, globals.PIECE_WHITE))
        # black pawn
        for move in [(1,1), (-1,1)]:
            newx, newy = x + move[0], y + move[1]
            if self.inbound((newx, newy)):
                occupied_by_king = self.board[newy][newx] is not None and self.board[newy][newx].type == globals.PIECE_KING
                if occupied_by_king:
                    found.append((newx,newy))
                elif self.board[newy][newx] is not None and self.board[newy][newx].type == globals.PIECE_PAWN:
                    found.append((newx,newy, globals.PIECE_BLACK))
        
        # gets any kings looking at the square
        for move in [(0,1), (0,-1), (1,0), (-1,0)]:
            newx, newy = x + move[0], y + move[1]
            if self.inbound((newx, newy)):
                if self.board[newy][newx] is not None and self.board[newy][newx].type == globals.PIECE_KING:
                    found.append((newx,newy))

        return found
    
    def get_sight_on_square_color(self, square, color):
        '''returns a list of all the pieces that attack a square from a specific color
        For example, all the black pieces that attack the square
        
        Keyword arguments:
        square -- the square
        color -- the color of the square
        '''
        sight = self.get_sight_on_square(square)
        purged = []
        
        for s in sight:
            if self.board[s[1]][s[0]].color == color:
                if len(s) == 3:
                    if (s[2] != color):
                        purged.append(s)
                else:
                    purged.append(s)
        return purged
    
    def in_check(self, color):
        '''returns if a king is in check
        
        Keyword arguments:
        color -- the color of the king
        '''
        if color == globals.PIECE_WHITE:
            return len(self.get_sight_on_square_color(self.get_king_pos(globals.PIECE_WHITE), globals.PIECE_BLACK)) > 0
        return len(self.get_sight_on_square_color(self.get_king_pos(globals.PIECE_BLACK), globals.PIECE_WHITE)) > 0
    
    def is_checkmate_or_stalemate(self, color):
        '''returns if the current board state is checkmate or stalemate for a color
        Keyword arguments:
        color -- the color you're looking for
        '''
        for y, row in enumerate(self.board):
            for x, grid_contents in enumerate(row):
                grid_empty = grid_contents is not None and grid_contents.type != globals.EN_PASSENT_FLAG
                if grid_empty and grid_contents.color == color:
                    if len(self.board[y][x].get_legal_moves(self, (x,y))) > 0:
                        return (False, "")
        if (self.in_check(color)):
            return (True, globals.CHECKMATE)
        return (True, globals.STALEMATE)

    
    def is_draw(self):
        '''returns if the game is a draw'''
        for pos in self.past_board_states:
            if self.past_board_states[pos] >= 3:
                return (True, "draw by threefold repition")

        has_sufficant = False
        tracker = {
            globals.PIECE_WHITE: {
                globals.PIECE_KNIGHT: -1,
                globals.PIECE_BISHOP: 0,
            },
            globals.PIECE_BLACK: {
                globals.PIECE_KNIGHT: -1,
                globals.PIECE_BISHOP: 0,
            }
        }
        for y, row in enumerate(self.board):
            for x, square in enumerate(row):
                if square is not None and square.type != globals.EN_PASSENT_FLAG and square.type != globals.PIECE_KING:
                    if square.type == globals.PIECE_BISHOP or square.type == globals.PIECE_KNIGHT:
                        tracker[square.color][square.type] += 1
                        knight_bishop = tracker[square.color][globals.PIECE_BISHOP] + 1 + tracker[square.color][globals.PIECE_KNIGHT]
                        if tracker[square.color][square.type] >= 2 or knight_bishop >= 2:
                            has_sufficant = True
                            break
                    else:
                        has_sufficant = True
                        break
            if has_sufficant:
                break
        if has_sufficant:
            return (self.fifty_move_rule_counter >= 100, "draw by 50 move rule")
        return (True, "draw by insufficant material")
    
    def search(self, start, direction, type):
        '''Search for a specific set of peices in a direction
        Used as a helper in get_sight_on_square
        
        Keyword arguments:
        start -- the starting position
        direction -- the vector of the search (eg, [0,-1] for up]
        type -- the peice types you're looking for
        '''
        x,y = direction
        factor = 1

        newx, newy = start[0] + (x*factor), start[1] + (y*factor)
        while self.inbound((newx, newy)):
            if self.board[newy][newx] is not None and self.board[newy][newx].type != globals.EN_PASSENT_FLAG:
                if self.board[newy][newx].type in type:
                    return (newx, newy)
                else:
                    return None
            factor += 1
            newx, newy = start[0] + (x*factor), start[1] + (y*factor)

        return None

    def make_legal_move(self, newpos, piece_location=None): 
        '''Makes a legal move, if the newpos argument is a legal move for the piece at piece_location

            Keyword arguments:
            newpos -- the target position of the piece
            piece_location -- the location of the old peice (default=None). If default, uses the selected square.
        '''
        newx, newy = newpos

        if piece_location is None:
            piece_location = (self.selected_square[0], self.selected_square[1])
        moves = self.board[piece_location[1]][piece_location[0]].get_legal_moves(self, pos=piece_location)
        if (newx, newy) in moves:
            self.move(piece_location, (newx, newy))
        
        # pawn moves
        elif (newx, newy, globals.NORMAL_FLAG) in moves:
            self.move(piece_location, (newx, newy))
            self.fifty_move_rule_counter = 0
        
        elif (newx, newy, globals.SHORT_CASTLE_FLAG) in moves:
            self.move(piece_location, (newx, newy))
            self.move((7, newy), (newx-1, newy), turn=0)
        
        elif (newx, newy, globals.LONG_CASTLE_FLAG) in moves:
            self.move(piece_location, (newx, newy))
            self.move((0, newy), (newx+1, newy), turn=0)
        
        elif (newx, newy, globals.DOUBLE_MOVE_FLAG) in moves:
            turn_color = globals.PIECE_WHITE if self.move_counter % 2 == 0 else globals.PIECE_BLACK
            offset = 1 if turn_color == globals.PIECE_WHITE else -1 
            self.move(piece_location, (newx, newy))
            self.board[newy+offset][newx] = ep.en_passent(self.move_counter,turn_color, newy)

        elif (newx, newy, globals.PROMOTION_FLAG) in moves:
            self.move(piece_location, (newx, newy))
            self.promotion = Promotion((newx, newy))
        
        elif (newx, newy, globals.EN_PASSENT_FLAG) in moves:
            offset = -1 if self.board[newy][newx].color == globals.PIECE_WHITE else 1
            self.board[newy+offset][newx] = None
            self.move(piece_location, (newx, newy))
        return 

    def move(self, piece_location, newpos, turn=1):
        ''' Makes a move

        Keyword arguments:
        piece_location -- the location of the piece
        newpos -- the new positon
        turn -- the amount the turn counter should be increased (Default=1) 
        '''
        piece = self.board[piece_location[1]][piece_location[0]]
        newx, newy = newpos
        # if it's a capture
        if self.board[newx][newy] is not None and self.board[newx][newy].type != globals.EN_PASSENT_FLAG:
            self.fifty_move_rule_counter = 0
        self.board[newy][newx] = piece
        self.board[piece_location[1]][piece_location[0]] = None
        self.move_counter += turn
        piece.has_moved = True

        if str(self.board) in self.past_board_states:
            self.past_board_states[str(self.board)] += 1
        else: 
            self.past_board_states[str(self.board)] = 1

        check_color = globals.PIECE_WHITE if piece.color == globals.PIECE_BLACK else globals.PIECE_BLACK
        if (self.is_checkmate_or_stalemate(check_color)[0]):
            self.game_over = True

        if (self.is_draw()[0]):
            self.game_over = True

    def ready_to_exit(self):
        return self.game_over
    
    def handle_key_press(self, event):
        return

    def exit(self):
        is_mate_or_stale_white = self.is_checkmate_or_stalemate(globals.PIECE_BLACK)
        if (is_mate_or_stale_white[0]):
            if is_mate_or_stale_white[1] == globals.STALEMATE:
                return ['end', 'Stalemate']
            return ['end', 'White has won']
        is_mate_or_stale_black = self.is_checkmate_or_stalemate(globals.PIECE_WHITE)
        if (is_mate_or_stale_black[0]):
            if is_mate_or_stale_black[1] == globals.STALEMATE:
                return ['end', 'Stalemate']
            return ['end', "Black has won"]
        draw = self.is_draw()
        if (draw[0]):
            return ["end", draw[1]]

    @run_engine
    def update(self):
        c = 0
        light_row = False
        window = pygame.display.get_surface() 
        window.fill('black')
        for y, row in enumerate(self.board):
            light_row = not light_row
            for x, column in enumerate(row):
                

                # leading with light
                if light_row:
                    if (c % 2 == 0):
                        window.blit(assets.light_square, (x*globals.grid_size,y*globals.grid_size))  
                    else:
                        window.blit(assets.dark_square, (x*globals.grid_size,y*globals.grid_size))
                else:
                    if (c % 2 == 0):
                        window.blit(assets.dark_square, (x*globals.grid_size,y*globals.grid_size))  
                    else:
                        window.blit(assets.light_square, (x*globals.grid_size,y*globals.grid_size))
                c += 1

                if column is not None:
                    column.show(x,y)

        if self.promotion is not None:
            self.promotion.show()

        if self.selected_square is not None:
            pygame.draw.rect(
                window, (255, 0, 0), 
                pygame.Rect(self.selected_square[0]*globals.grid_size, self.selected_square[1]*globals.grid_size, globals.grid_size, globals.grid_size),
                width=1
            )

    def self_copy(self):
        return Play_State(self.board)
    
    def __str__(self):
        final = ""

        for row in self.board:
            for square in row:
                if square is None:
                    final += "   "
                else:
                    final += square.color[0] + square.type[0] + " "
            final += "\n"
        return final


import pygame
import assets.assets as assets
import pieces.white_pieces as wp
import pieces.black_pieces as bp
import pieces.en_passent as ep
import constants.globals as globals
import constants.move_sets as mv
from board.decorators import wait_for_promotion
from board.promotion import Promotion

class Chess_Board():
    def __init__(self):
        self.set_board()

    def set_board(self):
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
        self.selected_square = None
        self.past_board_states = {}
        self.promotion = None

    def click(self, gridx, gridy):
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
            self.make_legal_move(gridx, gridy)
            self.selected_square = None
            return
        
    def get_king_pos(self, color):
        for y, colomn in enumerate(self.board):
            for x, square in enumerate(colomn):
                if square is not None and square.color == color and square.type == globals.PIECE_KING:
                    return (x,y)
        raise Exception("King not found.") 
    
    def get_turn(self):
        if self.move_counter % 2 == 0:
            return globals.PIECE_WHITE
        return globals.PIECE_BLACK
    
    def inbound(self, pos):
        x,y = pos
        return (x >= 0) and (y >= 0) and (x < 8) and (y < 8)
        
    def get_sight_on_square(self, square):
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
        for move in [(1,1), (-1,-1), (-1,1), (1,-1)]:
            newx, newy = x + move[0], y + move[1]
            if self.inbound((newx, newy)):
                if self.board[newy][newx] is not None and (self.board[newy][newx].type == globals.PIECE_PAWN or self.board[newy][newx].type == globals.PIECE_KING):
                    found.append((newx,newy))
        
        # gets any kings looking at the square
        for move in [(0,1), (0,-1), (1,0), (-1,0)]:
            newx, newy = x + move[0], y + move[1]
            if self.inbound((newx, newy)):
                if self.board[newy][newx] is not None and self.board[newy][newx].type == globals.PIECE_KING:
                    found.append((newx,newy))

        return found
    
    def get_sight_on_square_color(self, square, color):
        sight = self.get_sight_on_square(square)
        purged = []
        
        for s in sight:
            if self.board[s[1]][s[0]].color == color:
                purged.append(s)
        return purged
    
    def in_check(self, color):
        if color == globals.PIECE_WHITE:
            return len(self.get_sight_on_square_color(self.get_king_pos(globals.PIECE_WHITE), globals.PIECE_BLACK)) > 0
        return len(self.get_sight_on_square_color(self.get_king_pos(globals.PIECE_BLACK), globals.PIECE_WHITE)) > 0
    
    def search(self, start, direction, type):
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

    def make_legal_move(self, newx, newy): 
        moves = self.board[self.selected_square[1]][self.selected_square[0]].get_legal_moves(self)
        piece_location = (self.selected_square[0], self.selected_square[1])
        if (newx, newy) in moves or (newx, newy, globals.NORMAL_FLAG) in moves:
            self.move(piece_location, (newx, newy))
        
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
        piece = self.board[piece_location[1]][piece_location[0]]
        newx, newy = newpos
        self.board[newy][newx] = piece
        self.board[piece_location[1]][piece_location[0]] = None
        self.move_counter += turn
        piece.has_moved = True


    def update(self):
        c = 0
        light_row = False
        window = pygame.display.get_surface() 
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

    
                
                    


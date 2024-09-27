import pygame
import assets.assets as assets
import pieces.white_pieces as wp
import pieces.black_pieces as bp
import pieces.en_passent as ep
import globals

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
        self.waiting_for_promotion = False

    def click(self, gridx, gridy):
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
        
    def get_legal_moves(self):
        piece = self.board[self.selected_square[1]][self.selected_square[0]]
        moves = piece.getPossibleMoves(self.selected_square)
        oldx, oldy = self.selected_square

        # handles special conditions with the pawns
        if (piece.type == globals.PIECE_PAWN):
            # pawns cannot capture ahead of them
            forward_move = moves[0]
            if (self.board[forward_move[1]][forward_move[0]] is not None):
                moves = []
            
            # pawns capture to their sides
            if (piece.color == globals.PIECE_BLACK):
                for i in [(1,1), (-1,1)]:
                    possible_move = (oldx+i[0], oldy+i[1])
                    if (possible_move[0] >= 0 and possible_move[1] >= 0 and possible_move[0] < 8 and possible_move[1] < 8):
                        possible_move_grid = self.board[possible_move[1]][possible_move[0]]
                        if  possible_move_grid is not None and possible_move_grid.color == globals.PIECE_WHITE:
                            if possible_move_grid.type == globals.EN_PASSENT_FLAG and possible_move_grid.turn_num == self.move_counter:
                                moves.append((oldx+i[0], oldy+i[1], globals.EN_PASSENT_FLAG))
                            elif possible_move_grid.type != globals.EN_PASSENT_FLAG:
                                moves.append(possible_move) 
                # pawns can move twice on their first turn
                if oldy == 1:
                    moves.append((oldx,oldy+2, globals.DOUBLE_MOVE_FLAG))
                
            elif (piece.color == globals.PIECE_WHITE):
                for i in [(1,-1), (-1,-1)]:
                    possible_move = (oldx+i[0], oldy+i[1])
                    if (possible_move[0] >= 0 and possible_move[1] >= 0 and possible_move[0] < 8 and possible_move[1] < 8):
                        possible_move_grid = self.board[possible_move[1]][possible_move[0]]
                        if  possible_move_grid is not None and possible_move_grid.color == globals.PIECE_BLACK:
                            if possible_move_grid.type == globals.EN_PASSENT_FLAG and possible_move_grid.turn_num == self.move_counter:
                                moves.append((oldx+i[0], oldy+i[1], globals.EN_PASSENT_FLAG))
                            elif possible_move_grid.type != globals.EN_PASSENT_FLAG:
                                moves.append(possible_move) 
                if oldy == 6:
                    moves.append((oldx,oldy-2, globals.DOUBLE_MOVE_FLAG))
            
        
        # handle special king moves
        if (piece.type == globals.PIECE_KING and piece.has_moved == False):
            if (piece.color == globals.PIECE_WHITE):
                # white castling
                if (wp.rookL.has_moved == False):
                    moves.append((self.selected_square[0]-2, self.selected_square[1], globals.LONG_CASTLE_FLAG))
                if (wp.rookR.has_moved == False):
                    moves.append((self.selected_square[0]+2, self.selected_square[1], globals.SHORT_CASTLE_FLAG))
            elif (piece.color == globals.PIECE_BLACK):
                # black castling
                if (bp.rookL.has_moved == False):
                    moves.append((self.selected_square[0]-2, self.selected_square[1], globals.LONG_CASTLE_FLAG))
                if (bp.rookR.has_moved == False):
                    moves.append((self.selected_square[0]+2, self.selected_square[1], globals.SHORT_CASTLE_FLAG))


        # given all possibly legal moves, loop over and remove illegal ones
        purged_moves = []
        for move in moves:
            if (len(move) == 3):
                newx, newy, catcher = move
            else:
                newx, newy = move

            # This checks that there is nothing blocking you from moving to that square
            while (abs(oldx-newx) > 1 or abs(oldy-newy) > 1) and piece.hops == False:
                # walk across, taking the same path as the peice.  
                if oldx < newx:
                    oldx += 1
                if oldx > newx:
                    oldx -= 1
                if oldy < newy:
                    oldy += 1
                if oldy > newy:
                    oldy -= 1
                # if you meet another peice, the move is illegal
                board_square_not_empty = self.board[oldy][oldx] is not None
                if board_square_not_empty:
                    not_occupied_by_en_passent = self.board[oldy][oldx].type != globals.EN_PASSENT_FLAG
                    if not_occupied_by_en_passent:
                        continue
        
            if (self.board[newy][newx] is not None) and (piece.color == self.board[newy][newx].color) and (self.board[newy][newx].type != globals.EN_PASSENT_FLAG):
                continue

            purged_moves.append(move)
        return purged_moves

    def make_legal_move(self, newx, newy): 
        moves = self.get_legal_moves()
        piece_location = (self.selected_square[0], self.selected_square[1])
        if (newx, newy) in moves:
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
        if self.selected_square is not None:
            pygame.draw.rect(
                window, (255, 0, 0), 
                pygame.Rect(self.selected_square[0]*globals.grid_size, self.selected_square[1]*globals.grid_size, globals.grid_size, globals.grid_size),
                width=1
            )

    
                
                    


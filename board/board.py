import pygame
import assets.assets as assets
import pieces.white_pieces as wp
import pieces.black_pieces as bp
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
        if self.selected_square is None and self.board[gridy][gridx] is not None:
            if self.move_counter % 2 == 0 and self.board[gridy][gridx].color == "white": 
                self.selected_square = (gridx, gridy)
            elif self.move_counter % 2 != 0 and self.board[gridy][gridx].color == "black":
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
        if (piece.type == "pawn"):
            # pawns cannot capture ahead of them
            forward_move = moves[0]
            if (self.board[forward_move[1]][forward_move[0]] is not None):
                moves = []
            
            # pawns capture to their sides
            if (piece.color == "black"):
                for i in [(1,1), (-1,1)]:
                    possible_move = (oldx+i[0], oldy+i[1])
                    if (possible_move[0] >= 0 and possible_move[1] >= 0 and possible_move[0] < 8 and possible_move[1] < 8):
                        possible_move_grid = self.board[possible_move[1]][possible_move[0]]
                        if  possible_move_grid is not None and possible_move_grid.color == "white":
                            moves.append(possible_move)
            elif (piece.color == "white"):
                for i in [(1,-1), (-1,-1)]:
                    possible_move = (oldx+i[0], oldy+i[1])
                    if (possible_move[0] >= 0 and possible_move[1] >= 0 and possible_move[0] < 8 and possible_move[1] < 8):
                        possible_move_grid = self.board[possible_move[1]][possible_move[0]]
                        if  possible_move_grid is not None and possible_move_grid.color == "black":
                            moves.append(possible_move)
        
        # handle special king moves
        if (piece.type == "king" and piece.has_moved == False):
            if (piece.color == "white"):
                # white castling
                if (wp.rookL.has_moved == False):
                    moves.append((self.selected_square[0]-2, self.selected_square[1], "long castle"))
                if (wp.rookR.has_moved == False):
                    moves.append((self.selected_square[0]+2, self.selected_square[1], "short castle"))
            elif (piece.color == "black"):
                # black castling
                if (bp.rookL.has_moved == False):
                    moves.append((self.selected_square[0]-2, self.selected_square[1], "long castle"))
                if (bp.rookR.has_moved == False):
                    moves.append((self.selected_square[0]+2, self.selected_square[1], "short castle"))


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
                if self.board[oldy][oldx] is not None:
                    continue
        
            if (self.board[newy][newx] is not None) and piece.color == self.board[newy][newx].color:
                continue

            purged_moves.append(move)
        return purged_moves

    def make_legal_move(self, newx, newy): 
        moves = self.get_legal_moves()
        piece_location = (self.selected_square[0], self.selected_square[1])
        if (newx, newy) in moves:
            self.move(piece_location, (newx, newy))
        elif (newx, newy, "short castle") in moves:
            self.move(piece_location, (newx, newy))
            self.move((7, newy), (newx-1, newy), turn=0)
        elif (newx, newy, "long castle") in moves:
            self.move(piece_location, (newx, newy))
            self.move((0, newy), (newx+1, newy), turn=0)
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

    
                
                    


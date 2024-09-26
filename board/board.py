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
            self.move(gridx, gridy)
            self.selected_square = None
            return
        
    def get_legal_moves(self):
        # TODO: this function should be replaced with get_legal_moves which returns a list of all the moves a piece can make
        piece = self.board[self.selected_square[1]][self.selected_square[0]]
        moves = piece.getPossibleMoves(self.selected_square)
        oldx, oldy = self.selected_square

        if (piece.type == "pawn"):
            print("moving pawn")
            if (piece.color == "black"):
                for i in [(1,1), (-1,1)]:
                    possible_move = (oldx+i[0], oldy+i[1])
                    possible_move_grid = self.board[possible_move[1]][possible_move[0]]
                    if  possible_move_grid is not None and possible_move_grid.color == "white":
                        moves.append(possible_move)
            elif (piece.color == "white"):
                for i in [(1,-1), (-1,-1)]:
                    possible_move = (oldx+i[0], oldy+i[1])
                    possible_move_grid = self.board[possible_move[1]][possible_move[0]]
                    if  possible_move_grid is not None and possible_move_grid.color == "black":
                        moves.append(possible_move)
        print(moves)
        purged_moves = []
        for move in moves:
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
        
            # TODO: add a check to see your taking your own peice
            if (self.board[newy][newx] is not None) and piece.color == self.board[newy][newx].color:
                continue

            purged_moves.append(move)
        return purged_moves

    def move(self, newx, newy): 
        moves = self.get_legal_moves()
        if (newx, newy) in moves:
            piece = self.board[self.selected_square[1]][self.selected_square[0]]
            self.board[newy][newx] = piece
            self.board[self.selected_square[1]][self.selected_square[0]] = None
            self.move_counter += 1
            piece.has_moved = True
        return    

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

    
                
                    


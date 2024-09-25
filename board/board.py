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
        self.is_white_turn = True
        self.move_counter = 0
        self.selected_square = None
        self.past_board_states = {}
        self.waiting_for_promotion = False

    def click(self, gridx, gridy):
        # TODO: make it so that a peice can only be selected on it's own turn.
        if self.selected_square == (gridx, gridy):
            return

        if self.selected_square is None and self.board[gridy][gridx] is not None:
            self.selected_square = (gridx, gridy)
            return
        
        if self.selected_square != None:
            self.move(gridx, gridy)
            self.selected_square = None
            return
        
    def move(self, newx, newy):
        piece = self.board[self.selected_square[1]][self.selected_square[0]]
        
        # This checks that there is nothing blocking you from moving to that square
        oldx, oldy = self.selected_square
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
                print(self.board[oldy][oldx])
                return

        # TODO: add a check to see your taking your own peice

        self.board[newy][newx] = piece
        self.board[self.selected_square[1]][self.selected_square[0]] = None
        return
        # TODO: move up when getPossibleMoves is implemented
        if (newx, newy) not in piece.getPossibleMoves():
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

    
                
                    


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
        if self.selected_square is not None and self.board[gridy][gridx] is not None:
            self.selected_square = (gridx, gridy)
            return
        
        if self.selected_square != None:
            self.move(gridx, gridy)
            self.selected_square = None
            return
        
    def move(self, gridx, gridy):
        self.board[gridy][gridx] = self.board[self.selected_square[1]][self.selected_square[0]]
        self.board[self.selected_square[1]][self.selected_square[0]] = None

    def update(self):
        c = 0
        light_row = False
        for y, row in enumerate(self.board):
            light_row = not light_row
            for x, column in enumerate(row):
                window = pygame.display.get_surface() 

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


    
                
                    


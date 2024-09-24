import pygame
import assets.assets as assets

class Chess_Board():
    def __init__(self):
        self.set_board()

    def set_board(self):
        self.board = [
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None],
            [None, None, None, None, None, None, None, None]
        ]

    
    def update(self):
        c = 0
        light_row = False
        for i, row in enumerate(self.board):
            light_row = not light_row
            print(light_row, i)
            for j, column in enumerate(row):
                window = pygame.display.get_surface() 

                # leading with light
                if light_row:
                    if (c % 2 == 0):
                        window.blit(assets.light_square, (j*128,i*128))  
                    else:
                        window.blit(assets.dark_square, (j*128,i*128))
                else:
                    if (c % 2 == 0):
                        window.blit(assets.dark_square, (j*128,i*128))  
                    else:
                        window.blit(assets.light_square, (j*128,i*128))
                c += 1
                
                    


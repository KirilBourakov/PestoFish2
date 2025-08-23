import pygame
import game.constants.globals as globals

class EnPassent():
    def __init__(self, turn_num, color, placer):
        self.turn_num = turn_num
        self.color = color
        self.placer = placer
        self.type = globals.EN_PASSANT_FLAG
    
    def show(self, x, y):
        # window = pygame.display.get_surface()
        # pygame.draw.rect(
        #     window, (0, 255, 0), 
        #     pygame.Rect(x*globals.grid_size, y*globals.grid_size, globals.grid_size, globals.grid_size),
        #     width=1
        # )
        return

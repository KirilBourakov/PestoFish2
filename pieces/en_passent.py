import pygame, chess.constants.globals as globals

class en_passent():
    def __init__(self, turn_num, color, placer):
        self.turn_num = turn_num
        self.color = color
        self.placer = placer
        self.type = globals.EN_PASSENT_FLAG
    
    def show(self, x, y):
        # window = pygame.display.get_surface()
        # pygame.draw.rect(
        #     window, (0, 255, 0), 
        #     pygame.Rect(x*globals.grid_size, y*globals.grid_size, globals.grid_size, globals.grid_size),
        #     width=1
        # )
        return

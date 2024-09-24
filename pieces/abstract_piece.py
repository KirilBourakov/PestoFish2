import pygame
import globals

class Abstract_Piece():
    def __init__(self, asset, moveset):
        self.asset = asset
        self.moveset = moveset
        self.have_moved = False

    def show(self, x, y):
        window = pygame.display.get_surface()
        window.blit(self.asset, (x*globals.grid_size, y*globals.grid_size))

import pygame
import globals

class Abstract_Piece():
    def __init__(self, asset, moveset):
        self.asset = asset
        self.moveset = moveset
        self.have_moved = False

    def getPossibleMoves(self, pos):
        # takes in a tuple pos, representing the position of peices
        # the function should return all the theoretically possible moves on the board using moveset on pos to get them
        # the return should be a list, and should not include moves that take you off the board (indexing starts at 0, and ends at 7)
        pass

    def show(self, x, y):
        window = pygame.display.get_surface()
        window.blit(self.asset, (x*globals.grid_size+(globals.resize_num/2), y*globals.grid_size+(globals.resize_num/2)))

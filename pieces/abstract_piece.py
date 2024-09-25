import pygame
import globals

class Abstract_Piece():
    def __init__(self, asset, moveset):
        self.asset = asset
        self.moveset = moveset
        self.have_moved = False

    def getPossibleMoves(self, pos):
        moves_list = []
        for move in self.moveset:
            new_pos = pos + move
            if new_pos[0] >= 0 or new_pos[1] >= 0 or new_pos[0] <= 7 or new_pos[1] <= 7:   
                moves_list.append(new_pos)
        return moves_list

    def show(self, x, y):
        window = pygame.display.get_surface()
        window.blit(self.asset, (x*globals.grid_size+(globals.resize_num/2), y*globals.grid_size+(globals.resize_num/2)))

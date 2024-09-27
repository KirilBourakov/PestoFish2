import pygame
import constants.globals as globals

class Abstract_Piece():
    def __init__(self, type, asset, moveset, color, hops=False):
        self.type = type
        self.asset = asset
        self.moveset = moveset
        self.has_moved = False
        self.hops = hops
        self.color = color

    def getPossibleMoves(self, pos):
        moves_list = []
        for move in self.moveset:
            new_pos = (pos[0] + move[0], pos[1] + move[1])
            if (new_pos[0] >= 0) and (new_pos[1] >= 0) and (new_pos[0] <= 7) and (new_pos[1] <= 7):   
                if len(move) == 3:
                    moves_list.append(new_pos + (move[2],))
                else:
                    moves_list.append(new_pos,)
        return moves_list

    def show(self, x, y):
        window = pygame.display.get_surface()
        window.blit(self.asset, (x*globals.grid_size+(globals.resize_num/2), y*globals.grid_size+(globals.resize_num/2)))

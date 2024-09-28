import pygame
import assets.assets as assets
import constants.globals as globals
import pieces.white_pieces as wp
import pieces.black_pieces as bp

class Promotion():
    def __init__(self, pos):
        self.pos = pos
        self.assets = [wp.queen, wp.rookL, wp.knight, wp.bishop] if pos[1] == 0 else [bp.queen, bp.rookL, bp.knight, bp.bishop]
        self.direction = 1 if pos[1] == 0 else -1

    def show(self):
        gridx, gridy = self.pos
        window = pygame.display.get_surface()
        for asset in self.assets:
            window.blit(assets.promotion_circle, (gridx*globals.grid_size,gridy*globals.grid_size))
            asset.show(gridx, gridy)
            gridy += self.direction
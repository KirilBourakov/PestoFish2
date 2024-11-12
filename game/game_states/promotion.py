import pygame
import game.assets.assets as assets
import game.constants.globals as globals
import game.pieces.white_pieces as wp
import game.pieces.black_pieces as bp

class Promotion():
    def __init__(self, pos):
        self.pos = pos
        self.assets = [wp.queen, wp.rookL, wp.knight, wp.bishop] if pos[1] == 0 else [bp.queen, bp.rookL, bp.knight, bp.bishop]
        self.direction = 1 if pos[1] == 0 else -1
        self.y_positions = [
            self.pos[1],
            self.pos[1] + 1 * self.direction,
            self.pos[1] + 2 * self.direction,
            self.pos[1] + 3 * self.direction
        ]
        self.conversion = {
            7: 0,
            6: 1,
            5: 2,
            4: 3
        }

    def handle_click(self, pos, board_obj):
        mousex, mousey = pos

        if (mousex != self.pos[0]) or (mousey not in self.y_positions):
            return
        
        piece_index = self.conversion[mousey] if mousey in self.conversion else mousey
        board_obj.board[self.pos[1]][self.pos[0]] = self.assets[piece_index]
        board_obj.promotion = None

    def show(self):
        gridx, gridy = self.pos
        window = pygame.display.get_surface()
        for asset in self.assets:
            window.blit(assets.promotion_circle, (gridx*globals.grid_size,gridy*globals.grid_size))
            asset.show(gridx, gridy)
            gridy += self.direction

        mousepos = pygame.mouse.get_pos()
        mousex, mousey = int(mousepos[0]/globals.grid_size), int(mousepos[1]/globals.grid_size)
        if mousex == gridx and mousey in self.y_positions:
            pygame.draw.rect(
                window, (255, 0, 0), 
                pygame.Rect(mousex*globals.grid_size, mousey*globals.grid_size, globals.grid_size, globals.grid_size),
                width=1
            )
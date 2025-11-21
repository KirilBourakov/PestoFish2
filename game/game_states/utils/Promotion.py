import pygame
import game.assets.assets as assets
import game.constants.globals as globals
import game.pieces.white_pieces as wp
import game.pieces.black_pieces as bp

from game.game_states.utils.cAPI import PIECE, cAPI


class Promotion:
    def __init__(self, old_pos: tuple[int, int], new_pos: tuple[int, int]) -> None:
        self.old_pos = old_pos
        self.new_pos = new_pos

        self.options = (
            [
                PIECE.WHITE_QUEEN,
                PIECE.WHITE_ROOK,
                PIECE.WHITE_KNIGHT,
                PIECE.WHITE_BISHOP,
            ]
            if self._is_white()
            else [
                PIECE.BLACK_QUEEN,
                PIECE.BLACK_ROOK,
                PIECE.BLACK_KNIGHT,
                PIECE.BLACK_BISHOP,
            ]
        )
        self.direction = 1 if self._is_white() else -1

        self.y_positions = [self.new_pos[1] + i * self.direction for i in range(4)]

    def _is_white(self):
        return self.new_pos[1] == 0

    def get_promotion_choice(self, pos) -> PIECE:
        mousex, mousey = pos

        if (mousex != self.new_pos[0]) or (mousey not in self.y_positions):
            return

        piece_index = mousey if self._is_white() else abs(mousey - 7)
        return self.options[piece_index]

    def show(self) -> None:
        window = pygame.display.get_surface()

        # Hide Pawn
        if (self.old_pos[0] + self.old_pos[1]) % 2 == 0:
            window.blit(
                assets.light_square,
                (
                    self.old_pos[0] * globals.grid_size,
                    self.old_pos[1] * globals.grid_size,
                ),
            )
        else:
            window.blit(
                assets.dark_square,
                (
                    self.old_pos[0] * globals.grid_size,
                    self.old_pos[1] * globals.grid_size,
                ),
            )

        # Promotion
        gridx, gridy = self.new_pos
        for asset in self.options:
            window.blit(
                assets.promotion_circle,
                (gridx * globals.grid_size, gridy * globals.grid_size),
            )
            window.blit(
                cAPI.get_surface(asset),
                (
                    gridx * globals.grid_size + (globals.resize_num / 2),
                    gridy * globals.grid_size + (globals.resize_num / 2),
                ),
            )
            gridy += self.direction

        # Highlighting
        mousepos = pygame.mouse.get_pos()
        mousex, mousey = int(mousepos[0] / globals.grid_size), int(
            mousepos[1] / globals.grid_size
        )
        if mousex == gridx and mousey in self.y_positions:
            pygame.draw.rect(
                window,
                (255, 0, 0),
                pygame.Rect(
                    mousex * globals.grid_size,
                    mousey * globals.grid_size,
                    globals.grid_size,
                    globals.grid_size,
                ),
                width=1,
            )

import numpy as np
import pygame, copy
import game.assets.assets as assets
import game.pieces.white_pieces as wp
import game.pieces.black_pieces as bp
import game.pieces.EnPassent as ep
import game.constants.globals as globals
import game.constants.move_sets as mv
from game.game_states.utils.Promotion import Promotion
from game.game_states.AbstractState import AbstractState
from game.game_states.utils.cAPI import cAPI, PIECE
from game.game_states.utils.decorators import disable_on_engine_turn, run_engine


# for testing

class PlayState(AbstractState):
    def __init__(self):
        '''The constructor.
        
        Keyword arguments:
        board -- takes in a board position, and returns a board with a shallow copy of it (default=None)
        '''
        self.game_type = None
        self.bottom_text = None
        self.api = cAPI()
        self.promotion = None
        self.selected_square = None

    def enter(self, *args):
        '''Initialize all the instance variables to a starting state

        Keyword arguments:
        args -- a list of lists that contains the game type at index 0,0
        '''
        self.game_type = globals.GAME_TYPE_PVP if len(args) == 0 else args[0][0]
        self.api.clear()
        self.bottom_text = globals.WHITE_TO_MOVE
        self.promotion = None
        self.selected_square = None

    @disable_on_engine_turn
    def handle_click(self, gridx: int, gridy: int) -> None:
        '''Handles user clicks. Disabled when it's the engine's turn.
        
        Keyword arguments:
        gridx -- the x position of the click location on the board
        gridy -- the y position of the click location on the board
        '''

        if gridx >= 8 or gridy >= 8:
            return

        if self.promotion is not None:
            promote_to = self.promotion.get_promotion_choice((gridx, gridy))
            cAPI.make_move(self.promotion.old_pos, self.promotion.new_pos, promote_to)
            self.promotion = None
            return

        if self.selected_square is not None:
            if cAPI.is_legal_move(self.selected_square, (gridx, gridy)):
                white_promotion = gridy == 0 and cAPI.getAt(*self.selected_square) == PIECE.WHITE_PAWN
                black_promotion = gridy == 7 and cAPI.getAt(*self.selected_square) == PIECE.BLACK_PAWN
                if white_promotion or black_promotion:
                    self.promotion = Promotion(self.selected_square, (gridx, gridy))
                else:
                    cAPI.make_move(self.selected_square, (gridx, gridy), None)

        if self.selected_square is None and (cAPI.same_color(cAPI.active_color(), cAPI.getAt(gridx, gridy))):
            self.selected_square = (gridx, gridy)
        else:
            self.selected_square = None

        return

    @run_engine
    def update(self):
        c = 0
        light_row = False
        window = pygame.display.get_surface()
        window.fill('black')
        for y, row in enumerate(cAPI.get_curr_board()):
            light_row = not light_row
            for x, column in enumerate(row):
                # leading with light
                if light_row:
                    if c % 2 == 0:
                        window.blit(assets.light_square, (x * globals.grid_size, y * globals.grid_size))
                    else:
                        window.blit(assets.dark_square, (x * globals.grid_size, y * globals.grid_size))
                else:
                    if c % 2 == 0:
                        window.blit(assets.dark_square, (x * globals.grid_size, y * globals.grid_size))
                    else:
                        window.blit(assets.light_square, (x * globals.grid_size, y * globals.grid_size))
                c += 1

                surface = cAPI.get_surface(column)
                if surface is not None:
                    window.blit(surface, (x*globals.grid_size+(globals.resize_num/2), y*globals.grid_size+(globals.resize_num/2)))

        render = assets.text_large.render(self.bottom_text, False, "white")
        window.blit(render, (10, (8 * globals.grid_size)))

        if self.promotion is not None:
            self.promotion.show()

        if self.selected_square is not None:
            pygame.draw.rect(
                window, (255, 0, 0),
                pygame.Rect(self.selected_square[0] * globals.grid_size, self.selected_square[1] * globals.grid_size,
                            globals.grid_size, globals.grid_size),
                width=1
            )

    def handle_key_press(self, event):
        pass

    def ready_to_exit(self):
        return not cAPI.game_in_play()

    def exit(self):
        return ['end', cAPI.get_state_message()]

    def flip_bottom_text(self):
        if self.bottom_text == globals.WHITE_TO_MOVE:
            self.bottom_text = globals.BLACK_TO_MOVE
        elif self.bottom_text == globals.BLACK_TO_MOVE:
            self.bottom_text = globals.WHITE_TO_MOVE
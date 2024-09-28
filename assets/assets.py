import pygame, os
import constants.globals as globals

def prepare_surface(surface):
    return pygame.transform.scale(surface.convert_alpha(), (globals.grid_size, globals.grid_size))

def prepare_piece(surface):
    return pygame.transform.scale(surface.convert_alpha(), (globals.grid_size-globals.resize_num, globals.grid_size-globals.resize_num))

light_square = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'lightSquare.png'))
)
dark_square = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'darkSquare.png'))
)
promotion_circle = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'promotion.png'))
)

# TODO: added a promotion image sample we can use, find a way to implement later when promotion ruleset is added

""""
w_promotion_image = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__),  'imgs', 'w_promotion.png'))
)
b_promotion_image = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__),  'imgs', 'b_promotion.png'))
)
#sidenote: this code is not final. I just copied the format on the top and put it in there. Probably will need adjustments when the ruleset for it works.
"""


# white pieces
w_bishop = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_bishop.png'))
)
w_king = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_king.png'))
)
w_knight = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_knight.png'))
)
w_pawn = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_pawn.png'))
)
w_queen = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_queen.png'))
)
w_rook = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_rook.png'))
)

# black pieces
b_bishop = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'black', 'b_bishop.png'))
)
b_king = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'black', 'b_king.png'))
)
b_knight = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'black', 'b_knight.png'))
)
b_pawn = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'black', 'b_pawn.png'))
)
b_queen = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'black', 'b_queen.png'))
)
b_rook = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'black', 'b_rook.png'))
)






import pygame, os
import globals

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
w_rookl = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_rook.png'))
)
w_rookr = prepare_piece(
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
b_rookl = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'black', 'b_rook.png'))
)
b_rookr = prepare_piece(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'black', 'b_rook.png'))
)






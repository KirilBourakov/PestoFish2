import pygame, os

def prepare_surface(surface):
    return pygame.transform.scale(surface.convert_alpha(), (128, 128))

light_square = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'lightSquare.png'))
)
dark_square = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'darkSquare.png'))
)

# white pieces
w_bishop = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_bishop.png'))
)
w_king = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_king.png'))
)
w_knight = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_knight.png'))
)
w_pawn = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_pawn.png'))
)
w_queen = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_queen.png'))
)
w_rook = prepare_surface(
    pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_rook.png'))
)

# black piece





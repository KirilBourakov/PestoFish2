import pygame, os

light_square = pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'lightSquare.png')).convert_alpha()
dark_square = pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'darkSquare.png')).convert_alpha()

# white pieces
w_bishop = pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_bishop.png')).convert_alpha()
w_king = pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_king.png')).convert_alpha()
w_knight = pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_knight.png')).convert_alpha()
w_pawn = pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_pawn.png')).convert_alpha()
w_queen = pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_queen.png')).convert_alpha()
w_rook = pygame.image.load(os.path.join(os.path.dirname(__file__), 'imgs', 'white', 'w_rook.png')).convert_alpha()

# TODO: add black pieces





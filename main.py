import pygame

pygame.init()
pygame.display.set_caption('Chess')

window = pygame.display.set_mode((1024, 1024))
from board.board import Chess_Board

is_running = True
board = Chess_Board()

while is_running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            is_running = False
    board.update()
    pygame.display.update()
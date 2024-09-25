import pygame
import globals

pygame.init()
pygame.display.set_caption('Chess')
# TODO: have window be resizable, based on constant values

window = pygame.display.set_mode((globals.appsize, globals.appsize))
from board.board import Chess_Board


is_running = True
board = Chess_Board()

while is_running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            is_running = False

    mousepos = pygame.mouse.get_pos()
    gridx, gridy = int(mousepos[0]/globals.grid_size), int(mousepos[1]/globals.grid_size)
    if (pygame.mouse.get_pressed()[0] == True):
        board.click(gridx, gridy)
    board.update()
    pygame.display.update()
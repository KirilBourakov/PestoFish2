import pygame

pygame.init()
pygame.display.set_caption('Chess')

window = pygame.display.set_mode((1000, 1000))
import assets.assets as assets

is_running = True

while is_running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            is_running = False
    window.blit(assets.w_bishop, (10,10))
    pygame.display.update()
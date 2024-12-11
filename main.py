
def clean():
    import os
    os.environ['TF_CPP_MIN_LOG_LEVEL']='1'

def init_pygame():
    import pygame
    import game.constants.globals as globals
    pygame.mixer.init()
    pygame.init()
    pygame.display.set_mode((globals.appsize, globals.appsize+70))

    import game.assets.assets as assets
    pygame.display.set_caption('Chess')
    pygame.display.set_icon(assets.pesto)

def main():
    clean()
    init_pygame()

    import pygame
    import game.constants.globals as globals
    from game.game_states.StateManager import StateManager
    is_running = True
    manager = StateManager()

    while is_running:
        
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                is_running = False
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                mousepos = pygame.mouse.get_pos()
                gridx, gridy = int(mousepos[0]/globals.grid_size), int(mousepos[1]/globals.grid_size)
                manager.handle_click((gridx, gridy))
            if event.type == pygame.KEYDOWN:
                manager.handle_key_press(event)

        manager.update()
        pygame.display.update()


if __name__ == "__main__":
    main()
    
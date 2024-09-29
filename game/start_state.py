import pygame
from game.abstract_state import Abstract_State
import assets.assets as assets
import constants.globals as globals

class Start_State(Abstract_State):
    def __init__ (self):
        self.enter('')

    def enter(self, args):
        self.ready_to_exit_bool = False
        self.options = ["PVP", "Engine as black", "Engine as white"]
        self.chosen = self.options[0]

    def handle_click(self, x, y):
        self.ready_to_exit_bool = True

    def handle_key_press(self, event):
        if event.key == pygame.K_BACKSPACE:
            print("b")

    def ready_to_exit(self):
        return self.ready_to_exit_bool

    def exit(self):
        return ['play', self.chosen]
    
    def update(self):
        window = pygame.display.get_surface() 
        window.fill("black")

        render = assets.text_large.render("Chess", False, "white")
        x,y = assets.text_large.size("Chess")
        window.blit(render, (int((globals.appsize-x)/2), int((globals.appsize-y)/6)))

        move_factor = 3
        yprev = y * move_factor
        for option in self.options:
            render = assets.text_large.render(option, False, "white")
            x,y = assets.text_large.size(option)

            window.blit(render, (int((globals.appsize-x)/2), int((globals.appsize-y)/6)+yprev))
            
            move_factor += 1
            yprev = y * move_factor
            
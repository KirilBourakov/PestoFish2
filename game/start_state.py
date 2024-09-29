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
        self.chosen = 0

    def handle_click(self, x, y):
        return

    def handle_key_press(self, event):
        if event.key == pygame.K_UP:
            self.chosen = max(self.chosen-1, 0)
        elif event.key == pygame.K_DOWN:
            self.chosen = min(self.chosen+1, len(self.options)-1)
        # 13 represents enter that is not ony keypad
        elif event.key == 13 or event.key == pygame.K_KP_ENTER:
            self.ready_to_exit_bool = True

    def ready_to_exit(self):
        return self.ready_to_exit_bool

    def exit(self):
        return ['play', self.options[self.chosen]]
    
    def update(self):
        window = pygame.display.get_surface() 
        window.fill("black")

        render = assets.text_large.render("Chess", False, "white")
        x,y = assets.text_large.size("Chess")
        window.blit(render, (int((globals.appsize-x)/2), int((globals.appsize-y)/6)))

        offset = (y+50)
        for i, option in enumerate(self.options):
            render = assets.text_large.render(option, False, "white")
            horizontal_size, virtical_size= assets.text_large.size(option)

            xpos, ypos = int((globals.appsize-horizontal_size)/2), int((globals.appsize-virtical_size)/6)+offset
            window.blit(render, (xpos, ypos))
            offset += virtical_size

            # draw red bars to indicate selection
            if i == self.chosen:
                pygame.draw.rect(
                    window, (255, 0, 0), 
                    pygame.Rect(xpos-120, ypos+20, 80, virtical_size-30),
                )
                pygame.draw.rect(
                    window, (255, 0, 0), 
                    pygame.Rect(xpos+horizontal_size+40, ypos+20, 80, virtical_size-30),
                )

        offset += virtical_size

        render = assets.text_small.render("Hit <Enter> to Start", False, "white")
        x,y = assets.text_small.size("Hit <Enter> to Start")
        window.blit(render, (int((globals.appsize-x)/2), int((globals.appsize-y)/6)+offset))
            
from game.game_states.play_state import Play_State
from game.game_states.end_state import End_State
from game.game_states.start_state import Start_State

class State_Manager():
    def __init__(self):
        self.states = {
            "play": Play_State(),
            "end": End_State(),
            "start": Start_State()
        }
        self.current_state = "start"

    def change_state(self, new_state, args):
        if new_state in self.states.keys():
            self.current_state = new_state
            self.states[self.current_state].enter(args)
            return
        raise Exception("state not found")
    
    def handle_click(self, pos):
        self.states[self.current_state].handle_click(pos[0], pos[1])

    def handle_key_press(self, event):
         self.states[self.current_state].handle_key_press(event)

    def update(self):
        state = self.states[self.current_state]

        if state.ready_to_exit():
            response = state.exit()
            self.change_state(response[0], response[1:])

        state.update()
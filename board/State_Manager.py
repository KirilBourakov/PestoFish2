import board.board as board

class State_Manager():
    def __init__(self):
        self.states = {
            "play": board.Chess_Board()
        }
        self.current_state = "play"

    def change_state(self, new_state, args):
        if new_state in self.states.keys():
            self.current_state = new_state
            new_state.enter(args)
        raise Exception("state not found")
    
    def handle_click(self, pos):
        self.states[self.current_state].handle_click(pos[0], pos[1])

    def update(self):
        state = self.states[self.current_state]

        if state.ready_to_exit():
            response = state.exit()
            self.change_state(response[0], response[1:])

        state.update()
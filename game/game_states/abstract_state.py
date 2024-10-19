class Abstract_State():
    def enter(self):
        raise Exception("enter not implemented")

    def handle_click(self):
        raise Exception("handle_click not implemented")
    
    def handle_key_press(self):
        raise Exception("handle_key_press not implemented")
    
    def update(self):
        raise Exception("update not implemented")
    
    def ready_to_exit(self):
        raise Exception("ready_to_exit not implemented")

    def exit(self):
        raise Exception("exit not implemented")
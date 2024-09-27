class en_passent():
    def __init__(self, turn_num, color, placer):
        self.turn_num = turn_num
        self.color = color
        self.placer = placer
        self.type = "en passent"
    
    def show(self):
        return
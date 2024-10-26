

class engine():
    def __init__(self):
        generator = None #should contain a class that is used to generate moves
        evaluator = None #should contain a class that evaluates a given move position

    def accept_board(self, boardStr: str):
        split: list[str] = boardStr.split('/')
        self.fifty_move_rule_counter: int = int(split.pop())
        self.move_counter: int = int(split.pop())

        self.board: list[list[str]] = []

        for row in split:
            row = row.strip()
            s = row.split(" ")
            f_row = []
            for grid in s:
                f_row.append(grid.replace("--", "  "))
            self.board.append(f_row)
            

        for row in self.board:
            print(row)
        print(self.fifty_move_rule_counter)
        print(self.move_counter)
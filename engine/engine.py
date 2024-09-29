import constants.globals as globals
import random

def get_move_and_play(state, color):
    possible_moves = []

    for y, row in enumerate(state.board):
        for x, grid in enumerate(row):
            print(grid)
            if grid is not None and grid.type != globals.EN_PASSENT_FLAG and grid.color == color:
                moves = grid.get_legal_moves(state, (x,y))
                for move in moves:
                    possible_moves.append(((x,y), move))

    move = random.choice(possible_moves)
    state.move(move[0], move[1])
import constants.globals as globals
import random

def get_move_and_play(state, color):
    possible_moves = []

    for y, row in enumerate(state.board):
        for x, grid in enumerate(row):
            if grid is not None and grid.type != globals.EN_PASSENT_FLAG and grid.color == color:
                moves = grid.get_legal_moves(state, (x,y))
                for move in moves:
                    possible_moves.append(((x,y), (move[0], move[1])))
    try:
        move = random.choice(possible_moves)
        state.make_legal_move(move[1], piece_location=move[0])
    except IndexError:
        return
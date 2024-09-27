import constants.globals as globals
from constants.move_set_functions import double_move_possible, pawn_capture_possible, move_forward_possible, castle_possible


white_pawn_moves = [(0, -1, move_forward_possible), (0, -2, double_move_possible), (1,-1, pawn_capture_possible), (-1,-1, pawn_capture_possible)]
black_pawn_moves = [(0, +1, move_forward_possible), (0, +2, double_move_possible), (1,1, pawn_capture_possible), (-1,1, pawn_capture_possible)]


king_moves = [
    (-1, 1), (0, 1), (1,1),
    (-1,0), (1,0),
    (-1,-1), (0,-1), (1,-1),
    (-2, 0, castle_possible),
    (+2, 0, castle_possible)
]

knight_moves = [
    (+2, -1), (+2, +1),
    (-2, -1), (-2, +1),
    (-1, +2), (+1, +2),
    (-1, -2), (+1, -2),
]

rook_moves = []
for i in range(1, 16):
    rook_moves.append((i-8, 0))
    rook_moves.append((0, i-8))
rook_moves.remove((0,0))

bishop_moves = []
for i in range(1, 16):
    bishop_moves.append((i-8, i-8))
    bishop_moves.append((-(i-8), i-8))
bishop_moves.remove((0,0))
bishop_moves.remove((0,0))

queen_moves = rook_moves.copy()
queen_moves.extend(bishop_moves)
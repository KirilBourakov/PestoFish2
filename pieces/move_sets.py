white_pawn_moves = [(0, -1)]
black_pawn_moves = [(0, +1)]

king_moves = [
    (-1, 1), (0, 1), (1,1),
    (-1,0), (1,0),
    (-1,-1), (0,-1), (1,-1)
]

knight_moves = [
    (+3, -1), (+3, +1),
    (-3, -1), (-3, +1),
    (-1, +3), (+1, +3),
    (-1, -3), (+1, -3),
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
import globals

def pawn_capture_possible(pos, color, newpos, board, turn_num):
    on_the_board = (newpos[0] >= 0 and newpos[1] >= 0 and newpos[0] < 8 and newpos[1] < 8)
    if on_the_board:
        possible_move_grid = board[newpos[1]][newpos[0]]
        space_occupied_by_enemy = possible_move_grid is not None and possible_move_grid.color != color
        if space_occupied_by_enemy:
            # if it's an enpassent flag placed last turn
            en_passent_created_last_turn = possible_move_grid.type == globals.EN_PASSENT_FLAG and possible_move_grid.turn_num == turn_num
            if en_passent_created_last_turn:
                return (True, globals.EN_PASSENT_FLAG)
            elif possible_move_grid.type != globals.EN_PASSENT_FLAG:
                return (True, globals.NORMAL_CAPTURE_FLAG)
            
    return (False, '')

def double_move_possible(pos, color, *args):
    white_on_first_rank = (color == globals.PIECE_WHITE and pos[1] == 6)
    black_on_seventh_rank = (color == globals.PIECE_BLACK and pos[1] == 1)
    if (white_on_first_rank or black_on_seventh_rank):
        return (True, globals.DOUBLE_MOVE_FLAG)
    else:
        return (False, '')

white_pawn_moves = [(0, -1), (0, -2, double_move_possible), (1,-1, pawn_capture_possible), (-1,-1, pawn_capture_possible)]
black_pawn_moves = [(0, +1), (0, +2, double_move_possible), (1,1, pawn_capture_possible), (-1,1, pawn_capture_possible)]

king_moves = [
    (-1, 1), (0, 1), (1,1),
    (-1,0), (1,0),
    (-1,-1), (0,-1), (1,-1)
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
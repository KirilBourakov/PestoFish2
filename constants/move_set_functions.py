import constants.globals as globals

def pawn_capture_possible(pos, color, newpos, board, turn_num, *args):
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
                return (True, globals.NORMAL_FLAG)
            
    return (False, '')

def double_move_possible(pos, color, *args):
    white_on_first_rank = (color == globals.PIECE_WHITE and pos[1] == 6)
    black_on_seventh_rank = (color == globals.PIECE_BLACK and pos[1] == 1)
    if (white_on_first_rank or black_on_seventh_rank):
        return (True, globals.DOUBLE_MOVE_FLAG)
    else:
        return (False, '')
    
def move_forward_possible(pos, color, newpos, board, *args):
    if board[newpos[1]][newpos[0]] is not None and board[newpos[1]][newpos[0]].type != globals.EN_PASSENT_FLAG:
        return (False, '')
    return (True, globals.NORMAL_FLAG)
    
def castle_possible(pos, color, newpos, board, turn_num):
    newx, newy = newpos
    if newx == 2:
        path_is_clear = board[newy][1] is None and board[newy][2] is None
        if board[newy][0] is not None and board[newy][0].has_moved == False and path_is_clear:
            return (True, globals.LONG_CASTLE_FLAG)
    elif newx == 6:
        path_is_clear = board[newy][5] is None and board[newy][6] is None
        if board[newy][7] is not None and board[newy][7].has_moved == False and path_is_clear:
            return (True, globals.SHORT_CASTLE_FLAG)
    return (False, '')
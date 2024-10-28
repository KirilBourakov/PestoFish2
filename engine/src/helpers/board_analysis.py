from engine.src.constants.constants import BLACK, WHITE, QUEEN, ROOK, BISHOP, KNIGHT, KING, PAWN
from engine.src.constants.types import coloredPiecesList, peiceType
from engine.src.helpers.square_analysis import inbounds, is_empty, get_type, get_color

def sight_on_square(board: list[list[str]], location: tuple[int, int]) -> dict[str, list[tuple[int,int]]]:
    '''Returns a list of pieces in the form of a coloredPiecesList that can 'see' a location

    Keyword arguments:
    \t board - a chess board
    \t location - the (x,y) coordinates of a location you want examined
    '''

    final: dict[str, list[tuple[int,int]]] = {'b': [], 'w': []}

    # rook and queen
    directions: list[tuple[int,int]] = [(0,1), (0,-1), (1,0), (-1,0)]
    for direction in directions:
        result: peiceType = walk_search(board, location, direction, 8, [QUEEN, ROOK])
        print(result)
        if len(result['peice']) > 0:
            final[get_color(result['peice'])].append(result['location'])

    # bishop and queen
    directions = [(1,1), (-1,-1), (-1,1), (1,-1)]
    for direction in directions:
        result = walk_search(board, location, direction, 8, [BISHOP, ROOK])
        if len(result['peice']) > 0:
            final[get_color(result['peice'])].append(result['location'])

    # knight
    directions = [(+2, -1), (+2, +1),(-2, -1), (-2, +1),(-1, +2), (+1, +2),(-1, -2), (+1, -2)]
    for direction in directions:
        result = walk_search(board, location, direction, 8, [KNIGHT])
        if len(result['peice']) > 0:
            final[get_color(result['peice'])].append(result['location'])

    # king (not covered by pawn)
    directions = [(0,1), (0,-1), (1,0), (-1,0), (1,1), (-1,1), (1,-1), (-1,-1)]
    for direction in directions:
        result = walk_search(board, location, direction, 8, [KING])
        if len(result['peice']) > 0:
            final[get_color(result['peice'])].append(result['location'])
        
    # white pawn
    directions = [(1,-1), (-1,-1)]
    for direction in directions:
        result = walk_search(board, location, direction, 8, [PAWN], WHITE)
        if len(result['peice']) > 0:
            final[get_color(result['peice'])].append(result['location'])
    
    # black
    directions = [(1,1), (-1,1)]
    for direction in directions:
        result = walk_search(board, location, direction, 8, [PAWN], BLACK)
        if len(result['peice']) > 0:
            final[get_color(result['peice'])].append(result['location'])
    return final



def walk_search(board: list[list[str]], start: tuple[int, int], direction: tuple[int, int], max_force: int, type: list[str], color_restriction: str = '') -> peiceType:
        '''Starting at a specific sqaure, 'walk' in a specific direction, and return the pieces you find.
        
        Returns the first piece found in that direction
        
        Keyword arguments:
        \t board -- the chess board
        \t start -- the starting location
        \t direction -- the vector of the search (eg, [0,-1] for up]
        \t max_force -- the max force applied to direction
        \t type -- the pieces to look for
        \t color_restriction -- restrict search to specific color (no restriction by default)
        '''
        x,y = direction
        curr_force: int = 1
        newx, newy = start[0] + (x*curr_force), start[1] + (y*curr_force)
        piece: peiceType = {'peice': '', 'location': (-1,-1)}

        while curr_force <= max_force and inbounds((newx,newy)):
            if not is_empty(board[newy][newx]) and get_type(board[newy][newx]) in type:
                if color_restriction == '' or color_restriction == get_color(board[newy][newx]):
                    piece['peice'] = board[newy][newx]
                    piece['location'] = (newx, newy)
                break
            curr_force += 1
            newx, newy = start[0] + (x*curr_force), start[1] + (y*curr_force)

        return piece
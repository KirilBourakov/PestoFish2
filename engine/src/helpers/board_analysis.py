from engine.src.constants.constants import BLACK, WHITE, QUEEN, ROOK, BISHOP, KNIGHT, KING, PAWN
from engine.src.constants.types import pieceType
from engine.src.helpers.square_analysis import inbounds, is_empty, get_type, get_color

def find_king(board: list[list[str]], color: str):
        '''Finds a king of a certin color given a position'''
        for y, board_row in enumerate(board):
            for x, square in enumerate(board_row):
                if (get_type(square) == KING):
                    if get_color(square) == color:
                        return (x,y)
        raise IndexError('No King Index Found')

def sight_on_square(board: list[list[str]], location: tuple[int, int]) -> dict[str, list[tuple[int,int]]]:
    '''Returns a list of pieces in the form of a dictionary that can 'see' a location
    The dict keys are the color, and the list of tuple is the position the the pieces that can see the king

    Keyword arguments:
    \t board - a chess board
    \t location - the (x,y) coordinates of a location you want examined
    '''

    final: dict[str, list[tuple[int,int]]] = {'b': [], 'w': []}

    # rook and queen
    directions: list[tuple[int,int]] = [(0,1), (0,-1), (1,0), (-1,0)]
    for direction in directions:
        result: pieceType = walk_search(board, location, direction, 8, [QUEEN, ROOK])
        if len(result['piece']) > 0:
            final[get_color(result['piece'])].append(result['location'])

    # bishop and queen
    directions = [(1,1), (-1,-1), (-1,1), (1,-1)]
    for direction in directions:
        result = walk_search(board, location, direction, 8, [BISHOP, ROOK])
        if len(result['piece']) > 0:
            final[get_color(result['piece'])].append(result['location'])

    # knight
    directions = [(+2, -1), (+2, +1),(-2, -1), (-2, +1),(-1, +2), (+1, +2),(-1, -2), (+1, -2)]
    for direction in directions:
        result = walk_search(board, location, direction, 1, [KNIGHT])
        if len(result['piece']) > 0:
            final[get_color(result['piece'])].append(result['location'])

    # king (not covered by pawn)
    directions = [(0,1), (0,-1), (1,0), (-1,0), (1,1), (-1,1), (1,-1), (-1,-1)]
    for direction in directions:
        result = walk_search(board, location, direction, 1, [KING])
        if len(result['piece']) > 0:
            final[get_color(result['piece'])].append(result['location'])
        
    # black pawn
    directions = [(1,-1), (-1,-1)]
    for direction in directions:
        result = walk_search(board, location, direction, 1, [PAWN], BLACK)
        if len(result['piece']) > 0:
            final[get_color(result['piece'])].append(result['location'])
    
    # white
    directions = [(1,1), (-1,1)]
    for direction in directions:
        result = walk_search(board, location, direction, 1, [PAWN], WHITE)
        if len(result['piece']) > 0:
            final[get_color(result['piece'])].append(result['location'])
    return final



def walk_search(board: list[list[str]], start: tuple[int, int], direction: tuple[int, int], max_force: int, type: list[str], color_restriction: str = '') -> pieceType:
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
        piece: pieceType = {'piece': '', 'location': (-1,-1)}

        while curr_force <= max_force and inbounds((newx,newy)):
            if not is_empty(board[newy][newx]):
                type_searched_for: bool = get_type(board[newy][newx]) in type
                matches_color_restriction: bool = color_restriction == '' or color_restriction == get_color(board[newy][newx])
                if type_searched_for and matches_color_restriction:
                    piece['piece'] = board[newy][newx]
                    piece['location'] = (newx, newy)
                break
            curr_force += 1
            newx, newy = start[0] + (x*curr_force), start[1] + (y*curr_force)

        return piece
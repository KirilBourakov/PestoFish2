from engine.src.constants.types import coloredPiecesList
from engine.src.helpers.square_analysis import inbounds, is_empty

def sight_on_square(board: list[list[str]], location: tuple[int, int]) -> coloredPiecesList:
    '''Returns a list of pieces in the form of a coloredPiecesList that can 'see' a location

    Keyword arguments:
    \t board - a chess board
    \t location - the (x,y) coordinates of a location you want examined
    '''
    return



def walk_search(board: list[list[str]], start: tuple[int, int], direction: tuple[int, int], max_force: int) -> str:
        '''Starting at a specific sqaure, 'walk' in a specific direction, and return the pieces you find.
        
        Returns the first piece found in that direction
        
        Keyword arguments:
        \t board -- the chess board
        \t start -- the starting location
        \t direction -- the vector of the search (eg, [0,-1] for up]
        \t max_force -- the max force applied to direction
        '''
        x,y = direction
        curr_force = 1
        newx, newy = start[0] + (x*curr_force), start[1] + (y*curr_force)
        piece = ''

        while curr_force <= max_force and inbounds(newx,newy):
            if not is_empty(board[newy][newx]):
                piece = board[newy][newx]
                break
            curr_force += 1
            newx, newy = start[0] + (x*curr_force), start[1] + (y*curr_force)

        return piece
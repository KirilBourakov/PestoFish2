def get_color(piece: str) -> str:
    '''Give a piece, get the char that represents its color (will always be lowercase)'''
    return piece[0].lower()

def get_type(piece: str) -> str:
    '''Give a piece, get the char that represents its type'''
    return piece[1]

def has_moved(piece: str) -> bool:
    '''return if a given piece has moved. Empty squares return false.'''
    return piece[0].islower() and len(piece.strip()) > 0

def is_empty_include_en_passent(square: str) -> bool:
    '''returns if a square is empty. En passent is considered a non-empty square'''
    return len(square.strip()) == 0

def is_empty(square: str)-> bool:
    '''returns if a square is empty. En passent is considered a empty square'''
    return len(square.strip()) == 0 or get_type(square) == 'e'

def is_empty_squares(squares: list[str]) -> bool:
    '''returns if a any squares in a list are empty'''
    for square in squares:
        if (not is_empty(square)):
            return False
    return True

def inbounds(square: tuple[int, int]) -> bool:
    '''returns if a square is on the board'''
    return (square[0] >= 0) and (square[1] >= 0) and (square[0] <= 7) and (square[1] <= 7)
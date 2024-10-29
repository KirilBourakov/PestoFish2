from engine.src.constants.constants import BLACK, WHITE

def flip(color: str) -> str:
    '''given a color, return the opposite one'''
    return WHITE if color == BLACK else WHITE
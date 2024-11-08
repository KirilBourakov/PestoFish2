from engine.src.constants.static import BLACK, WHITE

def flip(color: str) -> str:
    '''given a color, return the opposite one'''
    return WHITE if color == BLACK else BLACK
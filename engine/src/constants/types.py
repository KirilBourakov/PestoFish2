from typing import TypedDict


# Represents the way a piece can move
Vector = TypedDict('Vector', {'maxForce': int, 'directions': list[tuple[int, int]]})

# Represents a list of pieces that can see a square by color
coloredPiecesList = TypedDict('coloredPiecesList', {'b': list[tuple[int, int]], 'w': list[tuple[int, int]]})

# Represents a piece and it's location
peiceType = TypedDict('peiceType', {'peice': str, 'location': tuple[int, int]})
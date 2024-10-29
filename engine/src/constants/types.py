from typing import TypedDict


# Represents the way a piece can move
Vector = TypedDict('Vector', {'maxForce': int, 'directions': list[tuple[int, int]]})

# Represents a piece and it's location
pieceType = TypedDict('pieceType', {'piece': str, 'location': tuple[int, int]})

# Represents the moves a piece can make
MoveType = TypedDict('MoveType', {'original': tuple[int, int], 'new': tuple[int, int], 'rating': int})
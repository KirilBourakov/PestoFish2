from typing import TypedDict


# Represents the way a piece can move
Vector = TypedDict('Vector', {'maxForce': int, 'directions': list[tuple[int, int]]})

# Represents a piece and it's location
pieceType = TypedDict('pieceType', {'piece': str, 'location': tuple[int, int]})
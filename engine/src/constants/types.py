from typing import TypedDict

# Represents the way a piece can move
Vector = TypedDict('Vector', {'maxForce': int, 'directions': list[tuple[int, int]]})

# Represents a list of pieces that can see a square by color
BoardAnalysis = TypedDict('BoardAnalysis', {'black_pieces': list[tuple[int, int]], 'white_pieces': list[tuple[int, int]]})
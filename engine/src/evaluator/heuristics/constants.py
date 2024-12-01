from engine.src.constants.static import PAWN, ROOK, BISHOP, KNIGHT, QUEEN, WHITE, KING, EMPTY, MIDDLE_GAME, END_GAME
import json
import os

piece_value_map: dict[str, dict[str, list[list[int]]]] = json.load(open(
    os.path.join(os.path.dirname(__file__), 'db.json')
))
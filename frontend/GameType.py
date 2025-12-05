from enum import Enum

import chess


class GameType(Enum):
    PLAYER_WHITE = 0
    PLAYER_BLACK = 1
    PVP = 3

    def is_playing(self, color: chess.Color | None) -> bool:
        if self == self.PVP:
            return True
        if color is None:
            return False
        elif color == chess.WHITE and self == self.PLAYER_WHITE:
            return True
        elif color == chess.BLACK and self == self.PLAYER_BLACK:
            return True
        return False
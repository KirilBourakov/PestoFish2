//
// Created by Kiril on 2025-08-27.
//

export module Board;
import Types;
import <cstdint>;

export constexpr int BOARD_SIZE = 8;

export BoardArray getStartingBoard();
export bool inBounds(int x, int y);
export bool sameColor(Color color, Piece piece);
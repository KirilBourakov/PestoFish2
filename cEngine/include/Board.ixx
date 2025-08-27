//
// Created by Kiril on 2025-08-27.
//

export module Board;
import Types;
import <cstdint>;

export constexpr std::int8_t BOARD_SIZE = 8;

export BoardArray get_start_board();
export bool inBounds(int8_t x, int8_t y);
export bool sameColor(Color color, Piece piece);
//
// Created by Kiril on 2025-08-23.
//

#ifndef BOARD_H
#define BOARD_H
#include <array>

#include "Types.h"

const int8_t BOARD_SIZE = 8;


BoardArray get_start_board();
bool inBounds(const int x, const int y);
bool sameColor(const Color color, const Piece piece);

#endif //BOARD_H

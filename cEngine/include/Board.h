//
// Created by Kiril on 2025-08-23.
//

#ifndef BOARD_H
#define BOARD_H
import <array>;

import Types;

const int8_t BOARD_SIZE = 8;


BoardArray get_start_board();
bool inBounds(int8_t x, int8_t y);
bool sameColor(Color color, Piece piece);

#endif //BOARD_H

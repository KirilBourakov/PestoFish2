//
// Created by Kiril on 2025-08-23.
//

#ifndef BOARD_H
#define BOARD_H
#include <array>

enum Squares {
    EMPTY = 0,

    WHITE_PAWN = 1,
    WHITE_KNIGHT = 2,
    WHITE_BISHOP = 3,
    WHITE_ROOK = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,

    BLACK_PAWN = 7,
    BLACK_KNIGHT = 8,
    BLACK_BISHOP = 9,
    BLACK_ROOK = 10,
    BLACK_QUEEN = 11,
    BLACK_KING = 12
};
using BoardArray = std::array<std::array<Squares, 8>, 8>;
using boardPostion = unsigned char;

BoardArray get_start_board();
int getY(boardPostion inp);
int getX(boardPostion inp);

#endif //BOARD_H

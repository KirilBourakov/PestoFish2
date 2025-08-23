//
// Created by Kiril on 2025-08-23.
//

#ifndef TYPES_H
#define TYPES_H
#include <array>
#include <cstdint>

enum Piece {
    EMPTY = 0,

    WHITE_PAWN = 1,
    WHITE_KNIGHT = 2,
    WHITE_BISHOP = 3,
    WHITE_ROOK = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,

    BLACK_PAWN = -1,
    BLACK_KNIGHT = -2,
    BLACK_BISHOP = -3,
    BLACK_ROOK = -4,
    BLACK_QUEEN = -5,
    BLACK_KING = -6
};
enum Color {
    BLACK = -1,
    WHITE = 1,
};

using BoardArray = std::array<std::array<Piece, 8>, 8>;

struct BoardPosition {
    int8_t x, y;
};

struct Move {
    BoardPosition start;
    BoardPosition end;
    Piece promotion = EMPTY;
};

#endif //TYPES_H

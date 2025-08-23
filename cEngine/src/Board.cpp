//
// Created by Kiril on 2025-08-23.
//

#include "../include/Board.h"

#include <stdexcept>
#include <vector>

BoardArray get_start_board() {
    return BoardArray{{
        {{BLACK_ROOK,   BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
        {{BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,  BLACK_PAWN, BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,  WHITE_PAWN, WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN}},
        {{WHITE_ROOK,   WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}
    }};
}

bool inBounds(int x) {
    return x >= 0 && x <= 7;
}
bool sameColor(Color color, Piece piece) {
    return (color == WHITE && piece > 0) || (color == BLACK && piece < 0);
}

void addSlidingMoves(BoardArray& board, int8_t x, int8_t y, std::vector<Move> &moves, bool straight, bool diag, Color color) {
    BoardPosition start {
        .x = x, .y = y
    };
    std::vector<std::pair<int8_t,int8_t>> dirs;
    if (straight && diag) {
        dirs = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    }
    else if (diag) {
        dirs = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    }
    else if (straight) {
        dirs = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    }
    else {
        throw std::invalid_argument("Invalid direction");
    }

    for (std::pair<int8_t,int8_t> [dy, dx] : dirs) {
        for (int i = 1; i <= BOARD_SIZE; i++) {
            int8_t newY = y + (i*dy);
            int8_t newX = x + (i*dx);
            if (!inBounds(newX) || !inBounds(newY)) {
                break;
            }

            if (sameColor(color, board[newY][newX])) {
                break;
            }

            Move newMove = {
                start,
                BoardPosition{
                    .x = newX, .y = newY
                },
            };

            moves.push_back(newMove);
            if (board[newY][newX] != EMPTY) {
                break;
            }
        }
    }
}
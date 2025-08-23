//
// Created by Kiril on 2025-08-23.
//

#include "../include/Board.h"

#include <optional>
#include <stdexcept>
#include <vector>
bool inBounds(const int x);

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

bool inBounds(const int x, const int y) {
    return inBounds(x) && inBounds(y);
}
bool inBounds(const int x) {
    return x >= 0 && x < BOARD_SIZE;
}

bool sameColor(const Color color, const Piece piece) {
    return (color == WHITE && piece > 0) || (color == BLACK && piece < 0);
}

Move createMove(const BoardPosition& start, int newX, int newY) {
    return Move {start,BoardPosition{.x = newX, .y = newY}};
}

void addPawnMoves(const BoardArray& board, const int8_t x, const int8_t y, const Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move> &moves) {
    int8_t dir = color == WHITE ? -1 : 1;
    int8_t newY = y + dir;
    BoardPosition start {
        .x = x, .y = y
    };
    // standered move
    if (inBounds(x,y) && board[newY][x] == EMPTY) {
        moves.push_back(createMove(start, x, newY));
    }

    // attacks
    std::vector<std::pair<int8_t,int8_t>> attacks = {{+1, dir}, {-1,dir}};
    for (auto [dx, dy] : attacks) {
        int8_t newX = x + dx;
        newY = y + dy;
        if (inBounds(newX, newY)) {
            if (board[newY][newX] != EMPTY && !sameColor(color, board[newY][newX])) {
                moves.push_back(createMove(start, newX, newY));
            }
        }
    }

    // enpassent
    if (enPassantSquare.has_value()) {
        bool correctX = std::abs(enPassantSquare->x - x) == 1;
        bool correctY = enPassantSquare->y == y + dir;
        if (correctX && correctY) {
            moves.push_back(createMove(start, enPassantSquare->x, enPassantSquare->y));
        }
    }

    // double move
    int8_t targetY = color == WHITE ? 6 : 1;
    if (y == targetY) {
        newY = y + 2 * dir;
        if (inBounds(x) && inBounds(newY)) {
            if (board[newY][x] == EMPTY) {
                moves.push_back(createMove(start, x, newY));
            }
        }
    }
}

void addKnightMoves(const BoardArray& board, const int8_t x, const int8_t y, const Color color, std::vector<Move> &moves) {
    static const std::vector<std::pair<int8_t,int8_t>> possible = {{+2, -1}, {+2, +1},{-2, -1}, {-2, +1},{-1, +2}, {+1, +2},{-1, -2}, {+1, -2}};
    BoardPosition start {
        .x = x, .y = y
    };
    for (auto [off_x, off_y] : possible) {
        int8_t newX = x + off_x, newY = y + off_y;
        if (inBounds(newX, newY)) {
            if (!sameColor(color, board[newY][newX])) {
                moves.push_back(createMove(start, newX, newY));
            }
        }
    }
}

void addSlidingMoves(BoardArray& board, int8_t x, int8_t y, std::vector<Move> &moves, bool straight, bool diag, Color color) {
    static const std::vector<std::pair<int8_t,int8_t>> straight_diag = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    static const std::vector<std::pair<int8_t,int8_t>> diag_dir = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    static const std::vector<std::pair<int8_t,int8_t>> straight_dir = {{0,1}, {0,-1}, {1,0}, {-1,0}};;

    BoardPosition start {
        .x = x, .y = y
    };
    const std::vector<std::pair<int8_t,int8_t>>* dirs;
    if (straight && diag) {
        dirs = &straight_diag;
    }
    else if (diag) {
        dirs = &diag_dir;
    }
    else if (straight) {
        dirs = &straight_dir;
    }
    else {
        throw std::invalid_argument("Invalid direction");
    }

    for (auto [dx, dy] : dirs) {
        for (int i = 1; i < BOARD_SIZE; i++) {
            int8_t newY = y + (i*dy);
            int8_t newX = x + (i*dx);
            if (!inBounds(newX, newY)) {
                break;
            }

            if (sameColor(color, board[newY][newX])) {
                break;
            }

            moves.push_back(createMove(start, newX, newY));
            if (board[newY][newX] != EMPTY) {
                break;
            }
        }
    }
}
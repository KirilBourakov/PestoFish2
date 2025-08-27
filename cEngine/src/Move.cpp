//
// Created by Kiril on 2025-08-23.
//
module;
#include <iostream>
#include <stdexcept>

module Move;
import <optional>;
import <vector>;
import Types;
import Utils;
import Board;

using moveSet = std::vector<std::pair<int8_t,int8_t>>;

Move createMove(const BoardPosition& start, const int8_t newX, const int8_t newY) {
    return Move {start,BoardPosition{.x = newX, .y = newY}};
}

void addKingMoves(const BoardArray& board, const int8_t x, const int8_t y, const Color color, const int castleRights, std::vector<Move> &moves) {
    static const moveSet straight_diag = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    const BoardPosition start {
        .x = x, .y = y
    };
    for (auto [dx, dy] : straight_diag) {
        const int8_t newX = x + dx;
        const int8_t newY = y + dy;
        if (inBounds(newX, newY) && !sameColor(color, board[newY][newX])) {
            moves.push_back(createMove(start, newX, newY));
        }
    }

    const int8_t newY = color == BLACK ? 0 : 7;
    if (castleAllowed(color, SHORT, castleRights)) {
        constexpr int8_t newX = 6;
        Move newMove = createMove(start, newX, newY);
        newMove.castle = SHORT;
        moves.push_back(newMove);
    }
    if (castleAllowed(color, LONG, castleRights)) {
        constexpr int8_t newX = 2;
        Move newMove = createMove(start, newX, newY);
        newMove.castle = LONG;
        moves.push_back(newMove);
    }
}

void addPawnMoves(const BoardArray& board, const int8_t x, const int8_t y, const Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move> &moves) {
    int8_t dir = color == WHITE ? -1 : 1;
    int8_t newY = y + dir;
    const BoardPosition start {
        .x = x, .y = y
    };
    // standered move
    if (inBounds(x,newY) && board[newY][x] == EMPTY) {
        moves.push_back(createMove(start, x, newY));
    }

    // attacks
   moveSet attacks = {{+1, dir}, {-1,dir}};
    for (auto [dx, dy] : attacks) {
        int8_t newX = x + dx;
        newY = y + dy;
        if (inBounds(newX, newY)) {
            if (board[newY][newX] != EMPTY && !sameColor(color, board[newY][newX])) {
                moves.push_back(createMove(start, newX, newY));
            }
        }
    }

    // en passent
    if (enPassantSquare.has_value()) {
        const bool correctX = std::abs(enPassantSquare->x - x) == 1;
        const bool correctY = enPassantSquare->y == y + dir;
        if (correctX && correctY) {
            moves.push_back(createMove(start, enPassantSquare->x, enPassantSquare->y));
        }
    }

    // double move
    int8_t startY = color == WHITE ? 6 : 1;
    if (y == startY) {
        newY = y + 2 * dir;
        if (inBounds(x, newY)) {
            if (board[newY][x] == EMPTY) {
                moves.push_back(createMove(start, x, newY));
            }
        }
    }
}

void addKnightMoves(const BoardArray& board, const int8_t x, const int8_t y, const Color color, std::vector<Move> &moves) {
    static const moveSet possible = {{+2, -1}, {+2, +1},{-2, -1}, {-2, +1},{-1, +2}, {+1, +2},{-1, -2}, {+1, -2}};
    const BoardPosition start {
        .x = x, .y = y
    };
    for (auto [off_x, off_y] : possible) {
        const int8_t newX = x + off_x;
        const int8_t newY = y + off_y;
        if (inBounds(newX, newY)) {
            if (!sameColor(color, board[newY][newX])) {
                moves.push_back(createMove(start, newX, newY));
            }
        }
    }
}

void addSlidingMoves(BoardArray& board, int8_t x, int8_t y, std::vector<Move> &moves, const bool straight, const bool diag, const Color color) {
    static const moveSet straight_diag = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    static const moveSet diag_dir = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    static const moveSet straight_dir = {{0,1}, {0,-1}, {1,0}, {-1,0}};

    const BoardPosition start {
        .x = x, .y = y
    };
    const moveSet* dirs;
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

    for (auto [dx, dy] : *dirs) {
        for (int i = 1; i < BOARD_SIZE; i++) {
            const int8_t newY = y + i*dy;
            const int8_t newX = x + i*dx;
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
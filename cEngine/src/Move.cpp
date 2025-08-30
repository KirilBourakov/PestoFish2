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

using moveSet = std::vector<std::pair<int,int>>;

Move createMove(const BoardPosition& start, const int newX, const int newY) {
    return Move {start,BoardPosition{.x = newX, .y = newY}};
}

void addKingMoves(const BoardArray& board, const int x, const int y, const Color color, const int castleRights, std::vector<Move> &moves) {
    static const moveSet straight_diag = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    const BoardPosition start {
        .x = x, .y = y
    };
    for (auto [dx, dy] : straight_diag) {
        const int newX = x + dx;
        const int newY = y + dy;
        if (inBounds(newX, newY) && !sameColor(color, board[newY][newX])) {
            moves.push_back(createMove(start, newX, newY));
        }
    }

    const int newY = color == BLACK ? 0 : 7;
    if (castleAllowed(color, SHORT, castleRights)) {
        constexpr int newX = 6;
        if (board[newY][newX-1] == EMPTY && board[newY][newX] == EMPTY) {
            Move newMove = createMove(start, newX, newY);
            newMove.castle = SHORT;
            moves.push_back(newMove);
        }
    }
    if (castleAllowed(color, LONG, castleRights)) {
        constexpr int newX = 2;
        if (board[newY][newX-1] == EMPTY && board[newY][newX] == EMPTY && board[newY][newX+1] == EMPTY) {
            Move newMove = createMove(start, newX, newY);
            newMove.castle = LONG;
            moves.push_back(newMove);
        }
    }
}

void addPawnMoves(const BoardArray& board, const int x, const int y, const Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move> &moves) {
    int dir = color == WHITE ? -1 : 1;
    int newY = y + dir;
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
        int newX = x + dx;
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
    int startY = color == WHITE ? 6 : 1;
    if (y == startY) {
        newY = y + 2 * dir;
        if (inBounds(x, newY)) {
            if (board[newY][x] == EMPTY) {
                Move m = createMove(start, x, newY);
                const int enPassantSquareY = newY-dir;
                m.enPassant = BoardPosition{x, enPassantSquareY};
                moves.push_back(m);
            }
        }
    }
}

void addKnightMoves(const BoardArray& board, const int x, const int y, const Color color, std::vector<Move> &moves) {
    static const moveSet possible = {{+2, -1}, {+2, +1},{-2, -1}, {-2, +1},{-1, +2}, {+1, +2},{-1, -2}, {+1, -2}};
    const BoardPosition start {
        .x = x, .y = y
    };
    for (auto [off_x, off_y] : possible) {
        const int newX = x + off_x;
        const int newY = y + off_y;
        if (inBounds(newX, newY)) {
            if (!sameColor(color, board[newY][newX])) {
                moves.push_back(createMove(start, newX, newY));
            }
        }
    }
}

void addSlidingMoves(const BoardArray& board, int x, int y, const Color color, const bool straight, const bool diag, std::vector<Move> &moves) {
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
            const int newY = y + i*dy;
            const int newX = x + i*dx;
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


bool isInCheck(const BoardArray &board, const BoardPosition kingPosition) {
    if (!inBounds(kingPosition.x, kingPosition.y) || std::abs(board[kingPosition.y][kingPosition.x]) != WHITE_KING) {
        throw std::invalid_argument("King position does not contain king or is out of bounds.");
    }

    const Color color = board[kingPosition.y][kingPosition.x] > 0 ? WHITE : BLACK;

    static const moveSet straight_dir = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    for (auto [dx, dy] : straight_dir) {
        for (int i = 1; i < BOARD_SIZE; i++) {
            const int newY = kingPosition.y + i*dy;
            const int newX = kingPosition.x + i*dx;
            if (!inBounds(newX, newY) || sameColor(color, board[newY][newX])) {
                break;
            }
            if (std::abs(board[newY][newX]) == WHITE_ROOK || std::abs(board[newY][newX]) == WHITE_QUEEN) {
                return true;
            }
            if (board[newY][newX] != EMPTY) {
                break;
            }
        }
    }

    static const moveSet diag_dir = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for (auto [dx, dy] : diag_dir) {
        for (int i = 1; i < BOARD_SIZE; i++) {
            const int newY = kingPosition.y + i*dy;
            const int newX = kingPosition.x + i*dx;
            if (!inBounds(newX, newY) || sameColor(color, board[newY][newX])) {
                break;
            }
            if (std::abs(board[newY][newX]) == WHITE_BISHOP || std::abs(board[newY][newX]) == WHITE_QUEEN) {
                return true;
            }
            if (board[newY][newX] != EMPTY) {
                break;
            }
        }
    }

    static const moveSet knightHops = {{+2, -1}, {+2, +1},{-2, -1}, {-2, +1},{-1, +2}, {+1, +2},{-1, -2}, {+1, -2}};
    for (auto [dx, dy] : knightHops) {
        const int newY = kingPosition.y + dy;
        const int newX = kingPosition.x + dx;
        if (inBounds(newX, newY) && !sameColor(color, board[newY][newX]) && std::abs(board[newY][newX]) == WHITE_KNIGHT) {
            return true;
        }
    }
    // check pawn
    int dir = color == WHITE ? -1 : 1;
    moveSet pawnAttacks = {{+1, dir}, {-1,dir}};
    for (auto [dx, dy] : pawnAttacks) {
        const int newY = kingPosition.y + dy;
        const int newX = kingPosition.x + dx;
        if (inBounds(newX, newY) && !sameColor(color, board[newY][newX]) && std::abs(board[newY][newX]) == WHITE_PAWN) {
            return true;
        }
    }

    // KINGS
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int newX = kingPosition.x + dx;
            int newY = kingPosition.y + dy;
            if (inBounds(newX, newY) && !sameColor(color, board[newY][newX]) &&std::abs(board[newY][newX]) == WHITE_KING) {
                return true;
            }
        }
    }

    return false;
}
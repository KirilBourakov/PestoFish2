//
// Created by Kiril on 2025-08-27.
//
module;
#include <iostream>

export module Board;
import Enums;
import <cstdint>;
import <array>;

export constexpr int BOARD_SIZE = 8;

export using BoardArray = std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE>;
export struct BoardPosition {
    int x, y;
    bool operator==(const BoardPosition &other) const {
        return x == other.x && y == other.y;
    }
};
export inline std::ostream& operator<<(std::ostream& os, const BoardPosition& pos) {
    return os << static_cast<char>(pos.x + 97) << (abs(pos.y - 8)) << "(" << pos.x << "," << pos.y << ")";
}

export BoardArray getStartingBoard() {
    using namespace Pieces;
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

export bool inBounds(const int x) {
    return x >= 0 && x < BOARD_SIZE;
}
export bool inBounds(const int x, const int y) {
    return inBounds(x) && inBounds(y);
}

export bool sameColor(const Color color, const Pieces::Piece piece) {
    if (piece == Pieces::EMPTY) {
        return false;
    }
    return Pieces::piece_color(piece) == color;
}

export bool castleAllowed(const Color color, const CastleType type, const int castleRights) {
    int bit;
    if (type == CastleType::SHORT && color == Color::White) {
        bit = 3;
    } else if (type == CastleType::LONG && color == Color::White) {
        bit = 2;
    } else if (type == CastleType::SHORT && color == Color::Black) {
        bit = 1;
    } else {
        bit = 0;
    }
    return castleRights >> bit & 1;
}
export void disAllowCastle(const Color color, const CastleType type, int& castleRights) {
    int bit;
    if (type == CastleType::SHORT && color == Color::White) {
        bit = 3;
    } else if (type == CastleType::LONG && color == Color::White) {
        bit = 2;
    } else if (type == CastleType::SHORT && color == Color::Black) {
        bit = 1;
    } else {
        bit = 0;
    }
    castleRights &= ~(1 << bit);
}
export void allowCastle(const Color color, const CastleType type, int& castleRights) {
    int bit;
    if (type == CastleType::SHORT && color == Color::White) {
        bit = 3;
    } else if (type == CastleType::LONG && color == Color::White) {
        bit = 2;
    } else if (type == CastleType::SHORT && color == Color::Black) {
        bit = 1;
    } else {
        bit = 0;
    }
    castleRights |= (1 << bit);
}


//
// Created by Kiril on 2025-08-27.
//

export module Types;

import <array>;
import <cstdint>;
import <optional>;

export enum Piece {
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

export enum Color {
    BLACK = -1,
    WHITE = 1,
};

export enum CastleType {
    SHORT = 0,
    LONG = 1
};

export using BoardArray = std::array<std::array<Piece, 8>, 8>;

export struct BoardPosition {
    int8_t x, y;
};

export struct Move {
    BoardPosition start;
    BoardPosition end;
    std::optional<Piece> promotion = std::nullopt;
    std::optional<CastleType> castle = std::nullopt;

    bool operator==(const Move& other) const {
        return other.start.x == start.x && other.start.y == start.y
            && other.end.x == end.x && other.end.y == end.y
            && promotion == other.promotion && castle == other.castle;
    }
};
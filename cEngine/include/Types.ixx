//
// Created by Kiril on 2025-08-27.
//
module;
#include <iostream>

export module Types;

import <array>;
import <cstdint>;
import <optional>;
import <string>;


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
    int x, y;
    bool operator==(const BoardPosition &other) const {
        return x == other.x && y == other.y;
    }
};
export inline std::ostream& operator<<(std::ostream& os, const BoardPosition& pos) {
    return os << "(" << pos.x << "," << pos.y << ")";
}

export struct Move {
    // TODO: track captured
    BoardPosition start{};
    BoardPosition end{};
    Piece endPiece;

    // TODO: we'd need to store the enPassent square, and a bool to tell us if it's capturing enPassent
    std::optional<Piece> promotion = std::nullopt;
    std::optional<CastleType> castle = std::nullopt;
    std::optional<BoardPosition> enPassant = std::nullopt;

    bool operator==(const Move& other) const {
        return other.start == start && other.end == end
            && promotion == other.promotion && castle == other.castle && enPassant == other.enPassant && endPiece == other.endPiece;
    }
};
export inline std::ostream& operator<<(std::ostream& os, const Move& m) {
    return os << "(" << m.start <<" -> " << m.end << "[" << std::to_string(m.endPiece) << "]";
}

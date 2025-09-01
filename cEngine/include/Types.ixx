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
    BoardPosition start{}; // starting square
    BoardPosition end{}; // end square
    Piece endPiece; // piece on old square

    bool enPassantCapture = false;
    std::optional<Piece> promotedTo = std::nullopt; // piece being promoted to
    std::optional<CastleType> castle = std::nullopt; // castle type
    std::optional<BoardPosition> newEnPassantSquare = std::nullopt; // location where en passant square now is

    static Move standardMove(BoardPosition start, BoardPosition end, Piece endPiece) {
        return {start, end, endPiece};
    }
    static Move promotionMove(BoardPosition start, BoardPosition end, Piece endPiece, Piece promotedTo) {
        return {start, end, endPiece, promotedTo};
    }
    static Move castleMove(BoardPosition start, BoardPosition end, Piece endPiece, CastleType castle) {
        return {start, end, endPiece, castle};
    }
    static Move doublePawnMove(BoardPosition start, BoardPosition end, Piece endPiece, BoardPosition enPassantSquare) {
        return {start, end, endPiece, enPassantSquare};
    }
    static Move enPassantCaptureMove(BoardPosition start, BoardPosition end, Piece endPiece){
        Move m = {start, end, endPiece};
        m.enPassantCapture = true;
        return m;
    }

    bool operator==(const Move& other) const {
        return other.start == start && other.end == end
            && promotedTo == other.promotedTo && castle == other.castle && newEnPassantSquare == other.newEnPassantSquare && endPiece == other.endPiece;
    }
private:
    Move() = default;
    Move(BoardPosition start, BoardPosition end, Piece endPiece) {
        this->start = start;
        this->end = end;
        this->endPiece = endPiece;
    }
    Move(BoardPosition start, BoardPosition end, Piece endPiece, Piece promotedTo) : Move(start, end, endPiece) {
        this->promotedTo = promotedTo;
    }
    Move(BoardPosition start, BoardPosition end, Piece endPiece, CastleType castle) : Move(start, end, endPiece) {
        this->castle = castle;
    }
    Move(BoardPosition start, BoardPosition end, Piece endPiece, BoardPosition enPassant) : Move(start, end, endPiece) {
        this->newEnPassantSquare = enPassant;
    }

};
export inline std::ostream& operator<<(std::ostream& os, const Move& m) {
    return os << "(" << m.start <<" -> " << m.end << "[" << std::to_string(m.endPiece) << "]";
}

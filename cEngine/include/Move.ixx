//
// Created by Kiril on 2025-08-27.
//
module;
#include <iostream>

export module Move;
import <optional>;
import <vector>;
import <string>;

import Enum;
import Board;


export struct Move {
    BoardPosition start{}; // starting square
    BoardPosition end{}; // end square

    bool enPassantCapture = false;
    std::optional<Piece> promotedTo = std::nullopt; // piece being promoted to
    std::optional<CastleType> castle = std::nullopt; // castle type
    std::optional<BoardPosition> newEnPassantSquare = std::nullopt; // location where en passant square now is

    static Move standardMove(BoardPosition start, BoardPosition end) {
        return {start, end};
    }
    static Move promotionMove(BoardPosition start, BoardPosition end, Piece promotedTo) {
        return {start, end, promotedTo};
    }
    static Move castleMove(BoardPosition start, BoardPosition end, CastleType castle) {
        return {start, end, castle};
    }
    static Move doublePawnMove(BoardPosition start, BoardPosition end, BoardPosition enPassantSquare) {
        return {start, end, enPassantSquare};
    }
    static Move enPassantCaptureMove(BoardPosition start, BoardPosition end){
        Move m = {start, end};
        m.enPassantCapture = true;
        return m;
    }

    bool operator==(const Move& other) const {
        return other.start == start && other.end == end
            && promotedTo == other.promotedTo && castle == other.castle && newEnPassantSquare == other.newEnPassantSquare;
    }
private:
    Move() = default;
    Move(BoardPosition start, BoardPosition end) {
        this->start = start;
        this->end = end;
    }
    Move(BoardPosition start, BoardPosition end, Piece promotedTo) : Move(start, end) {
        this->promotedTo = promotedTo;
    }
    Move(BoardPosition start, BoardPosition end, CastleType castle) : Move(start, end) {
        this->castle = castle;
    }
    Move(BoardPosition start, BoardPosition end, BoardPosition enPassant) : Move(start, end) {
        this->newEnPassantSquare = enPassant;
    }
};
export inline std::ostream& operator<<(std::ostream& os, const Move& m) {
    return os << m.start <<" -> " << m.end;
}

export void addPawnMoves(const BoardArray& board, const int x, const int y, const Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move> &moves);
export void addKingMoves(const BoardArray& board, const int x, const int y, const Color color, const int castleRights, std::vector<Move> &moves);
export void addKnightMoves(const BoardArray& board, const int x, const int y, const Color color, std::vector<Move> &moves);
export void addSlidingMoves(const BoardArray& board, int x, int y, const Color color, const bool straight, const bool diag, std::vector<Move> &moves);
export bool isAttacked(const BoardArray &board, const BoardPosition position);
export bool isAttacked(const BoardArray &board, const BoardPosition position, const Color color);
//
// Created by Kiril on 2025-08-27.
//
#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "ModuleOnly/Board.hpp"
#include "ModuleOnly/Enums.hpp"

struct Move {
    Move() = default;

    BoardPosition start{}; // starting square
    BoardPosition end{};   // end square

    bool enPassantCapture = false;
    std::optional<Pieces::Piece> promotedTo = std::nullopt;         // piece being promoted to
    std::optional<CastleType> castle = std::nullopt;                // castle type
    std::optional<BoardPosition> newEnPassantSquare = std::nullopt; // location where en passant square now is

    static Move standardMove(BoardPosition start, BoardPosition end) {
        return {start, end};
    }
    static Move promotionMove(BoardPosition start, BoardPosition end, Pieces::Piece promotedTo) {
        return {start, end, promotedTo};
    }
    static Move castleMove(BoardPosition start, BoardPosition end, CastleType castle) {
        return {start, end, castle};
    }
    static Move doublePawnMove(BoardPosition start, BoardPosition end, BoardPosition enPassantSquare) {
        return {start, end, enPassantSquare};
    }
    static Move enPassantCaptureMove(BoardPosition start, BoardPosition end) {
        Move m = {start, end};
        m.enPassantCapture = true;
        return m;
    }
    static Move invalid() {
        return {{-1, -1}, {-1, -1}};
    }

    bool operator==(const Move& other) const {
        return other.start == start && other.end == end && promotedTo == other.promotedTo && castle == other.castle &&
               newEnPassantSquare == other.newEnPassantSquare;
    }

private:
    Move(BoardPosition start, BoardPosition end) {
        this->start = start;
        this->end = end;
    }
    Move(BoardPosition start, BoardPosition end, Pieces::Piece promotedTo)
        : Move(start, end) {
        this->promotedTo = promotedTo;
    }
    Move(BoardPosition start, BoardPosition end, CastleType castle)
        : Move(start, end) {
        this->castle = castle;
    }
    Move(BoardPosition start, BoardPosition end, BoardPosition enPassant)
        : Move(start, end) {
        this->newEnPassantSquare = enPassant;
    }
};
inline std::ostream& operator<<(std::ostream& os, const Move& m) {
    return os << m.start << " -> " << m.end;
}

void addPawnMoves(const BoardArray& board, const int x, const int y, const Color color,
                  const std::optional<BoardPosition>& enPassantSquare, std::vector<Move>& moves);
void addKingMoves(const BoardArray& board, const int x, const int y, const Color color, const int castleRights,
                  std::vector<Move>& moves);
void addKnightMoves(const BoardArray& board, const int x, const int y, const Color color, std::vector<Move>& moves);
void addSlidingMoves(const BoardArray& board, int x, int y, const Color color, const bool straight, const bool diag,
                     std::vector<Move>& moves);
bool isAttacked(const BoardArray& board, const BoardPosition position);
bool isAttacked(const BoardArray& board, const BoardPosition position, const Color color);

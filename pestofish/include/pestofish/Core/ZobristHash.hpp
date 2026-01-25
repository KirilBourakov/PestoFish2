//
// Created by Kiril on 2025-10-26.
//
#pragma once

#include <array>
#include <optional>
#include <random>
#include <unordered_map>

#include "Board/Board.hpp"
#include "pestofish/Core/Move.hpp"

class ZobristHash {
public:
    ZobristHash(const Board& board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare);

    void flipActiveColor();
    void changeCastling(int prevCastlingRights, int newCastlingRights);
    void changeEnPassantSquare(std::optional<BoardPosition> prevEnPassantSquare, std::optional<BoardPosition> newEnPassantSquare);
    void makeMove(Move move, Pieces::Piece movedFrom, Pieces::Piece movedTo);

    [[nodiscard]] uint64_t getValue() const {
        return value;
    }
    void setValue(const uint64_t newValue) {
        value = newValue;
    }

    bool operator==(const ZobristHash& other) const {
        return value == other.value;
    }
    bool operator!=(const ZobristHash& other) const {
        return value != other.value;
    }

private:
    static unsigned long long seed;

    void recalculate(const Board& board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare);

    uint64_t value;

    // Generation aids
    std::mt19937_64 rng;
    std::uniform_int_distribution<unsigned long long> dis;
    std::unordered_map<Pieces::Piece, std::array<std::array<uint64_t, BOARD_SIZE>, BOARD_SIZE>> pieceTable;
    uint64_t blackToMove;
    std::array<uint64_t, 16> castleRights; // castling rights represented by int from ob1111 -> ob0000
    std::array<uint64_t, 8> enPassantSquare;
};

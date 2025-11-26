//
// Created by Kiril on 2025-10-26.
//
#pragma once

#include <array>
#include <optional>
#include <random>
#include <unordered_map>

#include "Board.hpp"
#include "ModuleOnly/Move.hpp"

using u64 = unsigned long long;

class ZobristHash {
public:
    ZobristHash(const Board& board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare);

    void flipActiveColor();
    void changeCastling(int prevCastlingRights, int newCastlingRights);
    void changeEnPassantSquare(std::optional<BoardPosition> prevEnPassantSquare, std::optional<BoardPosition> newEnPassantSquare);
    void makeMove(Move move, Pieces::Piece movedFrom, Pieces::Piece movedTo);

    [[nodiscard]] u64 getValue() const {
        return value;
    }
    void setValue(const u64 newValue) {
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

    u64 value;

    // Generation aids
    std::mt19937_64 rng;
    std::uniform_int_distribution<unsigned long long> dis;
    std::unordered_map<Pieces::Piece, std::array<std::array<u64, BOARD_SIZE>, BOARD_SIZE>> pieceTable;
    u64 blackToMove;
    std::array<u64, 16> castleRights; // castling rights represented by int from ob1111 -> ob0000
    std::array<u64, 8> enPassantSquare;
};

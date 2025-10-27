//
// Created by Kiril on 2025-10-26.
//
module;
#include <optional>
#include <unordered_map>

export module ZobristHash;
import Board;
import Enums;
import <array>;
import <random>;

using u64 = unsigned long long;

export class ZobristHash {
public:
    ZobristHash(const BoardArray &board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare);

    [[nodiscard]] unsigned long long getValue() const {return value;}

private:
    static unsigned long long seed;

    void recalculate(const BoardArray &board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare);

    u64 value;

    // Generation aids
    std::mt19937_64 rng;
    std::uniform_int_distribution<unsigned long long> dis;
    std::unordered_map<Pieces::Piece, std::array<std::array<u64, BOARD_SIZE>, BOARD_SIZE>> pieceTable;
    u64 blackToMove;
    std::array<u64, 16> castleRights; // castling rights represented by int from ob1111 -> ob0000
    std::array<u64, 8> enPassantSquare;
};

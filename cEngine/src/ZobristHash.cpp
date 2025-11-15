//
// Created by Kiril on 2025-10-26.
//
#include "ZobristHash.hpp"
#include <array>
#include <chrono>
#include <iostream>
#include <random>

#include "ModuleOnly/Board.hpp"

unsigned long long ZobristHash::seed = 90827521673ULL;

ZobristHash::ZobristHash(const NewBoard& board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantLocation)
    : rng(seed)
    , dis(std::numeric_limits<unsigned long long>::min(), std::numeric_limits<unsigned long long>::max()) {
    auto start = std::chrono::high_resolution_clock::now();
    using namespace Pieces;

    static std::array<Piece, 12> pieces{WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING,
                                        BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING};
    for (Piece piece : pieces) {
        std::array<std::array<u64, BOARD_SIZE>, BOARD_SIZE> values;
        for (auto i = 0; i < BOARD_SIZE; i++) {
            for (auto j = 0; j < BOARD_SIZE; j++) {
                values[i][j] = dis(rng);
            }
        }
        pieceTable[piece] = values;
    }
    blackToMove = dis(rng);
    for (auto i = 0; i < castleRights.size(); i++) {
        castleRights[i] = dis(rng);
    }
    for (auto i = 0; i < enPassantSquare.size(); i++) {
        enPassantSquare[i] = dis(rng);
    }

    recalculate(board, activeColor, castlingRights, enPassantLocation);
}

void ZobristHash::recalculate(const NewBoard& board, Color activeColor, int castlingRights,
                              std::optional<BoardPosition> enPassantLocation) {
    value = 0;
    for (int i = 0; i < board.size; i++) {
        for (int j = 0; j < board.size; j++) {
            if (Pieces::Piece piece = board(i, j); piece != Pieces::EMPTY) {
                value ^= pieceTable[piece][i][j];
            }
        }
    }
    if (activeColor == Color::Black) {
        value ^= blackToMove;
    }
    value ^= castleRights.at(castlingRights);
    if (enPassantLocation.has_value()) {
        value ^= enPassantSquare.at(enPassantLocation.value().x);
    }
}

void ZobristHash::flipActiveColor() {
    value ^= blackToMove;
}

void ZobristHash::changeCastling(const int prevCastlingRights, const int newCastlingRights) {
    value ^= castleRights.at(prevCastlingRights);
    value ^= castleRights.at(newCastlingRights);
}

void ZobristHash::changeEnPassantSquare(std::optional<BoardPosition> prevEnPassantSquare,
                                        std::optional<BoardPosition> newEnPassantSquare) {
    if (prevEnPassantSquare.has_value()) {
        value ^= enPassantSquare.at(prevEnPassantSquare.value().x);
    }
    if (newEnPassantSquare.has_value()) {
        value ^= enPassantSquare.at(newEnPassantSquare.value().x);
    }
}

void ZobristHash::makeMove(Move move, Pieces::Piece moved, Pieces::Piece movedTo) {
    if (movedTo != Pieces::EMPTY) {
        value ^= pieceTable[movedTo][move.end.y][move.end.x];
    }

    value ^= pieceTable[moved][move.start.y][move.start.x];
    value ^= pieceTable[moved][move.end.y][move.end.x];
}

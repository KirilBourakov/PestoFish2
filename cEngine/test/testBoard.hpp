//
// Created by Kiril on 2025-08-27.
//
#pragma once
#include "ModuleOnly/Enums.hpp"
#include "../include/Board/Board.hpp"

using namespace Pieces;

inline Board randomMiddleGame() {
    return Board({{{{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, EMPTY, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
                      {{BLACK_PAWN, EMPTY, BLACK_PAWN, BLACK_PAWN, EMPTY, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN}},
                      {{EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, EMPTY, EMPTY, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY}},
                      {{WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}}});
}

inline Board smotheredMatePosition() {
    return Board({{{{BLACK_ROOK, EMPTY, EMPTY, BLACK_BISHOP, BLACK_KING, BLACK_KNIGHT, EMPTY, BLACK_ROOK}},
                      {{BLACK_PAWN, BLACK_PAWN, EMPTY, BLACK_PAWN, BLACK_KNIGHT, BLACK_ROOK, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_KNIGHT, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, WHITE_PAWN, EMPTY, WHITE_PAWN}},
                      {{WHITE_ROOK, EMPTY, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, EMPTY, EMPTY, WHITE_ROOK}}}});
}

inline Board smotheredMateLongCastlePosition() {
    return Board({{{{BLACK_ROOK, EMPTY, EMPTY, BLACK_BISHOP, BLACK_KING, BLACK_KNIGHT, EMPTY, BLACK_ROOK}},
                      {{BLACK_PAWN, BLACK_PAWN, EMPTY, BLACK_PAWN, BLACK_KNIGHT, BLACK_ROOK, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_KNIGHT, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, WHITE_PAWN, EMPTY, WHITE_PAWN}},
                      {{WHITE_ROOK, EMPTY, EMPTY, EMPTY, WHITE_KING, WHITE_QUEEN, WHITE_BISHOP, WHITE_ROOK}}}});
}

inline Board unBlockedCastle() {
    return Board({{{{BLACK_ROOK, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, EMPTY}},
                      {{WHITE_ROOK, EMPTY, EMPTY, EMPTY, WHITE_KING, EMPTY, EMPTY, WHITE_ROOK}}}});
}

inline Board simpleMiddleGame() {
    return Board({{{{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_ROOK, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_QUEEN, WHITE_KING, EMPTY, EMPTY, EMPTY}}}});
}

inline Board knightCheck() {
    return Board({{{{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, WHITE_KNIGHT, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_ROOK, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, BLACK_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_QUEEN, WHITE_KING, EMPTY, EMPTY, EMPTY}}}});
}

inline Board straightCheck() {
    return Board({{{{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, WHITE_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, BLACK_QUEEN, EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY}}}});
}

inline Board diagCheck() {
    return Board({{{{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_BISHOP, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline Board pawnCheck() {
    return Board({{{{EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline Board kingCheck() {
    return Board({{{{EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline Board kingsFlankPawns() {
    return Board({{{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_KING, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline Board capturePromotion() {
    return Board({{{{BLACK_ROOK, BLACK_ROOK, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_PAWN, EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_KING, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline Board whiteBackRanked() {
    return Board({{{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_KING, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_KING, EMPTY, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline Board whiteSavedByBishop() {
    return Board({{{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_KING, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, WHITE_BISHOP, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_KING, EMPTY, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline Board centralWhiteKnight() {
    return Board({{{{EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_KNIGHT, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline Board knightMovedFromStart() {
    return Board({{{{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
                      {{BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN}},
                      {{WHITE_ROOK, EMPTY, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}}});
}

inline Board simpleBest() {
    return Board({{{{BLACK_ROOK, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, EMPTY}},
                      {{WHITE_ROOK, EMPTY, EMPTY, EMPTY, WHITE_KING, EMPTY, EMPTY, WHITE_ROOK}}}});
}

inline Board problemMiddleGame() {
    return Board({{{BLACK_ROOK, EMPTY, EMPTY, BLACK_BISHOP, EMPTY, EMPTY, EMPTY, BLACK_ROOK},
                      {BLACK_PAWN, BLACK_PAWN, EMPTY, BLACK_KING, BLACK_KNIGHT, BLACK_ROOK, BLACK_PAWN, BLACK_PAWN},
                      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KNIGHT, EMPTY},
                      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                      {WHITE_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY},
                      {WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_QUEEN, WHITE_PAWN, EMPTY, WHITE_PAWN},
                      {WHITE_ROOK, EMPTY, WHITE_BISHOP, EMPTY, WHITE_KING, EMPTY, EMPTY, WHITE_ROOK}}});
}

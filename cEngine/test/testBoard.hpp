//
// Created by Kiril on 2025-08-27.
//
#pragma once
#include "ModuleOnly/Board.hpp"
#include "ModuleOnly/Enums.hpp"

using namespace Pieces;

inline NewBoard randomMiddleGame() {
    return NewBoard({{{{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, EMPTY, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
                      {{BLACK_PAWN, EMPTY, BLACK_PAWN, BLACK_PAWN, EMPTY, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN}},
                      {{EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, EMPTY, EMPTY, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY}},
                      {{WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}}});
}

inline NewBoard smotheredMatePosition() {
    return NewBoard({{{{BLACK_ROOK, EMPTY, EMPTY, BLACK_BISHOP, BLACK_KING, BLACK_KNIGHT, EMPTY, BLACK_ROOK}},
                      {{BLACK_PAWN, BLACK_PAWN, EMPTY, BLACK_PAWN, BLACK_KNIGHT, BLACK_ROOK, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_KNIGHT, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, WHITE_PAWN, EMPTY, WHITE_PAWN}},
                      {{WHITE_ROOK, EMPTY, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, EMPTY, EMPTY, WHITE_ROOK}}}});
}

inline NewBoard smotheredMateLongCastlePosition() {
    return NewBoard({{{{BLACK_ROOK, EMPTY, EMPTY, BLACK_BISHOP, BLACK_KING, BLACK_KNIGHT, EMPTY, BLACK_ROOK}},
                      {{BLACK_PAWN, BLACK_PAWN, EMPTY, BLACK_PAWN, BLACK_KNIGHT, BLACK_ROOK, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_KNIGHT, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, WHITE_PAWN, EMPTY, WHITE_PAWN}},
                      {{WHITE_ROOK, EMPTY, EMPTY, EMPTY, WHITE_KING, WHITE_QUEEN, WHITE_BISHOP, WHITE_ROOK}}}});
}

inline NewBoard unBlockedCastle() {
    return NewBoard({{{{BLACK_ROOK, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, EMPTY}},
                      {{WHITE_ROOK, EMPTY, EMPTY, EMPTY, WHITE_KING, EMPTY, EMPTY, WHITE_ROOK}}}});
}

inline NewBoard simpleMiddleGame() {
    return NewBoard({{{{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_ROOK, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_QUEEN, WHITE_KING, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard knightCheck() {
    return NewBoard({{{{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, WHITE_KNIGHT, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_ROOK, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, BLACK_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_QUEEN, WHITE_KING, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard straightCheck() {
    return NewBoard({{{{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, WHITE_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, BLACK_QUEEN, EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard diagCheck() {
    return NewBoard({{{{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_BISHOP, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_QUEEN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard pawnCheck() {
    return NewBoard({{{{EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard kingCheck() {
    return NewBoard({{{{EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard kingsFlankPawns() {
    return NewBoard({{{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_KING, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard capturePromotion() {
    return NewBoard({{{{BLACK_ROOK, BLACK_ROOK, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_PAWN, EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_KING, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard whiteBackRanked() {
    return NewBoard({{{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_KING, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_KING, EMPTY, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard whiteSavedByBishop() {
    return NewBoard({{{{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, BLACK_KING, BLACK_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, WHITE_BISHOP, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_KING, EMPTY, BLACK_ROOK, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard centralWhiteKnight() {
    return NewBoard({{{{EMPTY, BLACK_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, WHITE_KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, WHITE_KNIGHT, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}}}});
}

inline NewBoard knightMovedFromStart() {
    return NewBoard({{{{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
                      {{BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN}},
                      {{WHITE_ROOK, EMPTY, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}}});
}

inline NewBoard simpleBest() {
    return NewBoard({{{{BLACK_ROOK, EMPTY, EMPTY, EMPTY, BLACK_KING, EMPTY, EMPTY, BLACK_ROOK}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, EMPTY, EMPTY, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, EMPTY, EMPTY}},
                      {{WHITE_ROOK, EMPTY, EMPTY, EMPTY, WHITE_KING, EMPTY, EMPTY, WHITE_ROOK}}}});
}

inline NewBoard problemMiddleGame() {
    return NewBoard({{{BLACK_ROOK, EMPTY, EMPTY, BLACK_BISHOP, EMPTY, EMPTY, EMPTY, BLACK_ROOK},
                      {BLACK_PAWN, BLACK_PAWN, EMPTY, BLACK_KING, BLACK_KNIGHT, BLACK_ROOK, BLACK_PAWN, BLACK_PAWN},
                      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_KNIGHT, EMPTY},
                      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                      {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                      {WHITE_KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE_PAWN, EMPTY},
                      {WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_QUEEN, WHITE_PAWN, EMPTY, WHITE_PAWN},
                      {WHITE_ROOK, EMPTY, WHITE_BISHOP, EMPTY, WHITE_KING, EMPTY, EMPTY, WHITE_ROOK}}});
}

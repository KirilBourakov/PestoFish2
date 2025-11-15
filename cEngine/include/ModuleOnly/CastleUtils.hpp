//
// Created by Kiril on 2025-08-27.
//
#pragma once
#include "Enums.hpp"

inline bool castleAllowed(const Color color, const CastleType type, const int castleRights) {
    int bit;
    if (type == CastleType::SHORT && color == Color::White) {
        bit = 3;
    } else if (type == CastleType::LONG && color == Color::White) {
        bit = 2;
    } else if (type == CastleType::SHORT && color == Color::Black) {
        bit = 1;
    } else {
        bit = 0;
    }
    return castleRights >> bit & 1;
}
inline void disAllowCastle(const Color color, const CastleType type, int& castleRights) {
    int bit;
    if (type == CastleType::SHORT && color == Color::White) {
        bit = 3;
    } else if (type == CastleType::LONG && color == Color::White) {
        bit = 2;
    } else if (type == CastleType::SHORT && color == Color::Black) {
        bit = 1;
    } else {
        bit = 0;
    }
    castleRights &= ~(1 << bit);
}
inline void allowCastle(const Color color, const CastleType type, int& castleRights) {
    int bit;
    if (type == CastleType::SHORT && color == Color::White) {
        bit = 3;
    } else if (type == CastleType::LONG && color == Color::White) {
        bit = 2;
    } else if (type == CastleType::SHORT && color == Color::Black) {
        bit = 1;
    } else {
        bit = 0;
    }
    castleRights |= (1 << bit);
}

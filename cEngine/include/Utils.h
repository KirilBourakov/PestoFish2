//
// Created by Kiril on 2025-08-23.
//

#ifndef UTILS_H
#define UTILS_H
#include "Types.h"

bool castleAllowed(Color color, CastleType type, int castleRights) {
    int bit;
    if (type == SHORT && color == WHITE) {
        bit = 3;
    } else if (type == LONG && color == WHITE) {
        bit = 2;
    } else if (type == SHORT && color == BLACK) {
        bit = 1;
    } else {
        bit = 0;
    }
    return (castleRights >> bit) & 1;
}
void disAllowCastle(Color color, CastleType type, int& castleRights) {
    int bit;
    if (type == SHORT && color == WHITE) {
        bit = 3;
    } else if (type == LONG && color == WHITE) {
        bit = 2;
    } else if (type == SHORT && color == BLACK) {
        bit = 1;
    } else {
        bit = 0;
    }
    castleRights &= ~(1 << bit);;
}

#endif //UTILS_H

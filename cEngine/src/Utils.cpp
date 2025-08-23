//
// Created by Kiril on 2025-08-23.
//

#include "Utils.h"

bool castleAllowed(const Color color, const CastleType type, const int castleRights) {
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
void disAllowCastle(const Color color, const CastleType type, int& castleRights) {
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
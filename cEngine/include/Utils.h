//
// Created by Kiril on 2025-08-23.
//

#ifndef UTILS_H
#define UTILS_H
#include "Types.h"

bool castleAllowed(Color color, CastleType type, int castleRights);
void disAllowCastle(Color color, CastleType type, int& castleRights);

#endif //UTILS_H

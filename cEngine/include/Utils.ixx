//
// Created by Kiril on 2025-08-27.
//

export module Utils;
import Types;

export bool castleAllowed(const Color color, const CastleType type, const int castleRights);
export void allowCastle(const Color color, const CastleType type, int& castleRights);
export void disAllowCastle(const Color color, const CastleType type, int& castleRights);

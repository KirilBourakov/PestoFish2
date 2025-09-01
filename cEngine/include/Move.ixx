//
// Created by Kiril on 2025-08-27.
//
module;
#include <iostream>

export module Move;
import Types;
import <optional>;
import <vector>;
import <string>;

export void addPawnMoves(const BoardArray& board, const int x, const int y, const Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move> &moves);
export void addKingMoves(const BoardArray& board, const int x, const int y, const Color color, const int castleRights, std::vector<Move> &moves);
export void addKnightMoves(const BoardArray& board, const int x, const int y, const Color color, std::vector<Move> &moves);
export void addSlidingMoves(const BoardArray& board, int x, int y, const Color color, const bool straight, const bool diag, std::vector<Move> &moves);
export bool isAttacked(const BoardArray &board, const BoardPosition position);
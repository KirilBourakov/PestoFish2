//
// Created by Kiril on 2025-08-23.
//

#ifndef MOVE_H
#define MOVE_H
#include <optional>
#include <vector>
#include "Types.h"

void addPawnMoves(const BoardArray& board, const int8_t x, const int8_t y, const Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move> &moves);
void addKnightMoves(const BoardArray& board, const int8_t x, const int8_t y, const Color color, std::vector<Move> &moves);
void addSlidingMoves(BoardArray& board, int8_t x, int8_t y, std::vector<Move> &moves, bool straight, bool diag, Color color);



#endif //MOVE_H

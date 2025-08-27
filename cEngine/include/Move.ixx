//
// Created by Kiril on 2025-08-27.
//

export module Move;
import Types;
import <optional>;
import <vector>;

export void addPawnMoves(const BoardArray& board, const int8_t x, const std::int8_t y, const Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move> &moves);
export void addKnightMoves(const BoardArray& board, const int8_t x, const int8_t y, const Color color, std::vector<Move> &moves);
export void addSlidingMoves(BoardArray &board, int8_t x, int8_t y, std::vector<Move> &moves, bool straight, bool diag,Color color);

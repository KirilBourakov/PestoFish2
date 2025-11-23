//
// Created by Kiril on 2025-11-14.
//

#pragma once
#include <array>
#include <format>
#include <vector>

#include "ModuleOnly/CastleUtils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"



constexpr int BOARD_SIZE = 8;
class NewBoard {
public:
    int size = BOARD_SIZE;

    explicit NewBoard(std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> inp)
        : board(inp) {}

    [[nodiscard]] std::vector<Move> getPseudoLegal(const Color activeColor, const std::optional<BoardPosition> enPassantSquare,
                                                   int castlingRights) const {
        std::vector<Move> moves;
        moves.reserve(200);
        // get all pseudo legal moves
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (Pieces::sameColor(activeColor, board[y][x])) {
                    addMoves(x, y, activeColor, enPassantSquare, castlingRights, moves);
                }
            }
        }
        return std::move(moves);
    }

    [[nodiscard]] Pieces::Piece at(const int y, const int x) const {
        return board[y][x];
    }
    void set(const Pieces::Piece piece, const int y, const int x) {
        board[y][x] = piece;
    }

    void addMoves(int x, int y, Color activeColor, std::optional<BoardPosition> enPassantSquare, int castlingRights, std::vector<Move>& out) const;
    void addKingMoves(int x, int y, Color color, int castleRights, std::vector<Move>& moves) const;
    static void addPromotions(BoardPosition start, BoardPosition end, Pieces::Piece piece, Color color, std::vector<Move>& moves);
    void addPawnMoves(int x, int y, Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move>& moves) const;
    void addKnightMoves(int x, int y, Color color, std::vector<Move>& moves) const;
    void addSlidingMoves(int x, int y, Color color, bool straight, bool diag, std::vector<Move>& moves) const;
    [[nodiscard]] bool isAttacked(const BoardPosition& position, Color color) const;
    [[nodiscard]] bool isAttacked(const BoardPosition& position) const;

    /**
     * @return True, if the board position is draw due to material. False otherwise.
     */
    [[nodiscard]] bool isDrawFromMaterial() const;

    /**
     * Gets a row at an index. Used to expose board to python.
     */
    [[nodiscard]] const auto& get_row(size_t i) const {
        return board[i];
    }

    friend std::ostream& operator<<(std::ostream& os, const NewBoard& b);
    bool operator==(const NewBoard& other) const {
        return board == other.board;
    }

    static bool inBounds(const int x) {
        return x >= 0 && x < BOARD_SIZE;
    }
    static bool inBounds(const int x, const int y) {
        return inBounds(x) && inBounds(y);
    }

private:
    std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> board;
};

/**
 * @return A board object in the starting position
 */
inline NewBoard getStartingBoard() {
    using namespace Pieces;
    return NewBoard({{{{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
                      {{BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN}},
                      {{WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}}});
}

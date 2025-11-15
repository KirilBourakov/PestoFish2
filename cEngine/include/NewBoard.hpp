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
        // get all pseudo legal moves
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (Pieces::sameColor(activeColor, board[y][x])) {
                    addMoves(x, y, activeColor, enPassantSquare, castlingRights, moves);
                }
            }
        }
        return moves;
    }
    void addMoves(int x, int y, Color activeColor, std::optional<BoardPosition> enPassantSquare, int castlingRights,
                  std::vector<Move>& out) const;

    bool isDrawFromMaterial() const {
        int wBishops = 0;
        int wKnights = 0;
        int bBishops = 0;
        int bKnights = 0;
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                switch (board[y][x]) {
                case Pieces::WHITE_PAWN:
                case Pieces::BLACK_PAWN:
                case Pieces::WHITE_ROOK:
                case Pieces::BLACK_ROOK:
                case Pieces::WHITE_QUEEN:
                case Pieces::BLACK_QUEEN:
                    return false;

                case Pieces::WHITE_KNIGHT:
                    wKnights++;
                    break;
                case Pieces::WHITE_BISHOP:
                    wBishops++;
                    break;
                case Pieces::BLACK_KNIGHT:
                    bKnights++;
                    break;
                case Pieces::BLACK_BISHOP:
                    bBishops++;
                    break;

                case Pieces::WHITE_KING:
                case Pieces::BLACK_KING:
                case Pieces::EMPTY:
                    break;
                }
            }
        }
        const bool whiteCanWin = (wBishops >= 1 && wKnights >= 1) || (wBishops >= 2) || (wKnights >= 3);
        const bool blackCanWin = (bBishops >= 1 && bKnights >= 1) || (bBishops >= 2) || (bKnights >= 3);
        return !whiteCanWin && !blackCanWin;
    }

    [[nodiscard]] bool isAttacked(const BoardPosition& position, Color color) const;
    [[nodiscard]] bool isAttacked(const BoardPosition& position) const;
    static bool inBounds(const int x) {
        return x >= 0 && x < BOARD_SIZE;
    }
    static bool inBounds(const int x, const int y) {
        return inBounds(x) && inBounds(y);
    }

    Pieces::Piece& operator()(const int y, const int x) {
        return board[y][x];
    }
    const Pieces::Piece& operator()(const int y, const int x) const {
        return board[y][x];
    }

    bool operator==(const NewBoard& other) const {
        return board == other.board;
    }

    const auto& get_row(size_t i) const {
        return board[i];
    }

    void print_board() const {
        std::cout << "  +-----------------+" << std::endl;
        for (int i = 0; i < BOARD_SIZE; i++) {
            std::cout << i + 1 << " | ";
            for (size_t j = 0; j < BOARD_SIZE; j++) {
                std::cout << Pieces::piece_char(board[i][j]) << ' ';
            }
            std::cout << '|' << std::endl;
        }
        std::cout << "  +-----------------+" << std::endl;
        std::cout << "    a b c d e f g h" << std::endl;
    }

    void addKingMoves(int x, int y, Color color, int castleRights, std::vector<Move>& moves) const;
    static void addPromotions(BoardPosition start, BoardPosition end, Pieces::Piece piece, Color color, std::vector<Move>& moves);
    void addPawnMoves(int x, int y, Color color, const std::optional<BoardPosition>& enPassantSquare, std::vector<Move>& moves) const;
    void addKnightMoves(int x, int y, Color color, std::vector<Move>& moves) const;
    void addSlidingMoves(int x, int y, Color color, bool straight, bool diag, std::vector<Move>& moves) const;

private:
    std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> board;
};

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

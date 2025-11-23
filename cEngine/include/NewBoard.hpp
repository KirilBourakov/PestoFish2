//
// Created by Kiril on 2025-11-14.
//

#pragma once
#include <array>
#include <cassert>
#include <format>
#include <vector>

#include "ModuleOnly/CastleUtils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"

constexpr int BOARD_SIZE = 8;

class BitBoardManager {
public:
    [[nodiscard]] uint64_t at(const Pieces::Piece piece) const {
        assert(piece != Pieces::EMPTY && "EMPTY has no bitboard");
        return board[indexOf(piece)];
    }

    [[nodiscard]] uint64_t at(const Color color) const {
        return colorBoard[static_cast<size_t>(color)]; // WHITE = 0, BLACK = 1
    }

    void insert(const Pieces::Piece piece, const int y, const int x) {
        assert(piece != Pieces::EMPTY && "EMPTY has no bitboard");

        const uint64_t mask = (1ULL << shiftValue(y, x));
        at_mut(piece) |= mask;
        at_mut(Pieces::piece_color(piece)) |= mask;
    }

    static int shiftValue(const int y, const int x) {
        return y * BOARD_SIZE + x;
    }

    bool operator==(const BitBoardManager& other) const {
        return board == other.board && colorBoard == other.colorBoard;
    }
    bool operator!=(const BitBoardManager& other) const {
        return !operator==(other);
    }

private:
    std::array<uint64_t, 12> board{};
    std::array<uint64_t, 2> colorBoard{};

    uint64_t& at_mut(const Color color) {
        return colorBoard[static_cast<size_t>(color)];
    }

    uint64_t& at_mut(const Pieces::Piece piece) {
        return board[indexOf(piece)];
    }

    static size_t indexOf(const Pieces::Piece piece) {
        switch (piece) {
            case Pieces::WHITE_PAWN:   return 0;
            case Pieces::WHITE_KNIGHT: return 1;
            case Pieces::WHITE_BISHOP: return 2;
            case Pieces::WHITE_ROOK:   return 3;
            case Pieces::WHITE_QUEEN:  return 4;
            case Pieces::WHITE_KING:   return 5;

            case Pieces::BLACK_PAWN:   return 6;
            case Pieces::BLACK_KNIGHT: return 7;
            case Pieces::BLACK_BISHOP: return 8;
            case Pieces::BLACK_ROOK:   return 9;
            case Pieces::BLACK_QUEEN:  return 10;
            case Pieces::BLACK_KING:   return 11;

            case Pieces::EMPTY:
            default:
                throw std::invalid_argument("EMPTY has no bitboard index");
        }
    }
};

class NewBoard {
public:
    explicit NewBoard(const std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> &inp)
        : board(inp)
    {
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (board[y][x] != Pieces::EMPTY) {
                    bitBoards.insert(board[y][x], y, x);
                }
            }
        }
    }

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

    void move(const Move& mv) {
        const Pieces::Piece newPiece = mv.promotedTo.value_or(this->at(mv.start.y, mv.start.x));

        board[mv.end.y][mv.end.x] = newPiece;
        board[mv.start.y][mv.start.x] = Pieces::EMPTY;

        if (mv.enPassantCapture) {
            board[mv.start.y][mv.end.x] = Pieces::EMPTY;
        } else if (mv.castle == CastleType::LONG) {
            const Pieces::Piece rook = this->at(mv.start.y, 0);
            board[mv.start.y][0] = Pieces::EMPTY;
            board[mv.start.y][mv.end.x + 1] = rook;
        } else if (mv.castle == CastleType::SHORT) {
            const Pieces::Piece rook = this->at(mv.start.y, 7);
            board[mv.start.y][7] = Pieces::EMPTY;
            board[mv.start.y][mv.end.x - 1] = rook;
        }
    }

    /**
     * Undoes a played Move
     * @param move Move played
     * @param movedPiece Piece Moved
     * @param overwrittenPiece Piece Taken or empty
     * @param activeColor The Color that played the Move
     */
    void undoMove(const Move& move, const Pieces::Piece movedPiece, const Pieces::Piece overwrittenPiece, const Color activeColor) {
        if (move.enPassantCapture) {
            board[move.start.y][move.end.x] = (activeColor == Color::White) ? Pieces::BLACK_PAWN : Pieces::WHITE_PAWN;
        } else if (move.castle == CastleType::LONG) {
            const Pieces::Piece rook = board[move.start.y][move.end.x + 1];
            board[move.start.y][move.end.x + 1] = Pieces::EMPTY;
            board[move.start.y][0] = rook;
        } else if (move.castle == CastleType::SHORT) {
            const Pieces::Piece rook = board[move.start.y][move.end.x - 1];
            board[move.start.y][move.end.x - 1] = Pieces::EMPTY;
            board[move.start.y][7] = rook;
        }

        board[move.end.y][move.end.x] = overwrittenPiece;
        board[move.start.y][move.start.x] = movedPiece;
    }

    [[nodiscard]] Pieces::Piece at(const int y, const int x) const {
        return board[y][x];
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

    static bool inBounds(const int x) {
        return x >= 0 && x < BOARD_SIZE;
    }
    static bool inBounds(const int x, const int y) {
        return inBounds(x) && inBounds(y);
    }

    friend std::ostream& operator<<(std::ostream& os, const NewBoard& b);
    bool operator==(const NewBoard& other) const {
        return board == other.board && bitBoards == other.bitBoards;
    }
    bool operator!=(const NewBoard& other) const {
        return !operator==(other);
    }

private:
    std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> board;
    BitBoardManager bitBoards{};
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

//
// Created by Kiril on 2025-11-14.
//

#pragma once
#include <array>
#include <cassert>
#include <format>
#include <vector>

#include "ModuleOnly/BitBoardManager.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"

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

        if (activeColor == Color::White) {
            bitBoards.addPawnMoves<Color::White>(enPassantSquare, moves);
            bitBoards.addKingMoves<Color::White>(castlingRights, moves);
            bitBoards.addKnightMoves<Color::White>(moves);
        }
        else {
            bitBoards.addPawnMoves<Color::Black>(enPassantSquare, moves);
            bitBoards.addKingMoves<Color::Black>(castlingRights, moves);
            bitBoards.addKnightMoves<Color::Black>(moves);
        }

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

    void move(const Move& mv, const Pieces::Piece startContent, const Pieces::Piece endContent) {
        bitBoards.move(mv, startContent, endContent);

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
        bitBoards.undoMove(move, movedPiece, overwrittenPiece, activeColor);

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

    BitBoardManager bitBoards{};
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

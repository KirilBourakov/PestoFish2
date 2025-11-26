//
// Created by Kiril on 2025-11-14.
//

#pragma once
#include <array>
#include <cassert>
#include <format>
#include <vector>

#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"

class ArrayBoard {
public:
    explicit ArrayBoard(const std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> &inp)
        : board(inp)
    {}

    void move(const Move& mv, const Pieces::Piece startContent, const Pieces::Piece endContent) {
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

    friend std::ostream& operator<<(std::ostream& os, const ArrayBoard& b);
    bool operator==(const ArrayBoard& other) const {
        return board == other.board;
    }
    bool operator!=(const ArrayBoard& other) const {
        return !operator==(other);
    }
private:
    std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> board;

};
//
// Created by Kiril on 2025-11-14.
//

#pragma once
#include <array>
#include <cassert>
#include <format>
#include <vector>

#include "pestofish/Core/Utils.hpp"
#include "pestofish/Core/Enums.hpp"
#include "pestofish/Core/Move.hpp"

class ArrayBoard {
public:
    explicit ArrayBoard(const std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> &inp)
        : board(inp)
    {}

    void move(const Move& mv, const Pieces::Piece startContent, const Pieces::Piece endContent) {
        const Pieces::Piece newPiece = mv.getPromotedTo().value_or(this->at(mv.getStart().y, mv.getStart().x));

        board[mv.getEnd().y][mv.getEnd().x] = newPiece;
        board[mv.getStart().y][mv.getStart().x] = Pieces::EMPTY;

        if (mv.getEnPassantCapture()) {
            board[mv.getStart().y][mv.getEnd().x] = Pieces::EMPTY;
        } else if (mv.getCastle() == CastleType::LONG) {
            const Pieces::Piece rook = this->at(mv.getStart().y, 0);
            board[mv.getStart().y][0] = Pieces::EMPTY;
            board[mv.getStart().y][mv.getEnd().x + 1] = rook;
        } else if (mv.getCastle() == CastleType::SHORT) {
            const Pieces::Piece rook = this->at(mv.getStart().y, 7);
            board[mv.getStart().y][7] = Pieces::EMPTY;
            board[mv.getStart().y][mv.getEnd().x - 1] = rook;
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
        if (move.getEnPassantCapture()) {
            board[move.getStart().y][move.getEnd().x] = (activeColor == Color::White) ? Pieces::BLACK_PAWN : Pieces::WHITE_PAWN;
        } else if (move.getCastle() == CastleType::LONG) {
            const Pieces::Piece rook = board[move.getStart().y][move.getEnd().x + 1];
            board[move.getStart().y][move.getEnd().x + 1] = Pieces::EMPTY;
            board[move.getStart().y][0] = rook;
        } else if (move.getCastle() == CastleType::SHORT) {
            const Pieces::Piece rook = board[move.getStart().y][move.getEnd().x - 1];
            board[move.getStart().y][move.getEnd().x - 1] = Pieces::EMPTY;
            board[move.getStart().y][7] = rook;
        }

        board[move.getEnd().y][move.getEnd().x] = overwrittenPiece;
        board[move.getStart().y][move.getStart().x] = movedPiece;
    }

    [[nodiscard]] Pieces::Piece at(const int y, const int x) const {
        return board[y][x];
    }

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
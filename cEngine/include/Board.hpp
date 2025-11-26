//
// Created by Kiril on 11/25/2025.
//

#pragma once
#include "NewBoard.hpp"
#include "ModuleOnly/BitBoardManager.hpp"

class Board {
public:
    explicit Board(const std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> &board)
        : newBoard(board)
        , bitBoard(board)
    {}

    [[nodiscard]] std::vector<Move> getPseudoLegal(const Color activeColor, const std::optional<BoardPosition> enPassantSquare,
                                                   int castlingRights) const {
        std::vector<Move> moves;
        moves.reserve(200);

        if (activeColor == Color::White) {
            bitBoard.addPawnMoves<Color::White>(enPassantSquare, moves);
            bitBoard.addKingMoves<Color::White>(castlingRights, moves);
            bitBoard.addKnightMoves<Color::White>(moves);
        }
        else {
            bitBoard.addPawnMoves<Color::Black>(enPassantSquare, moves);
            bitBoard.addKingMoves<Color::Black>(castlingRights, moves);
            bitBoard.addKnightMoves<Color::Black>(moves);
        }

        // get all pseudo legal moves
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (Pieces::sameColor(activeColor, at(y,x))) {
                    newBoard.addMoves(x, y, activeColor, enPassantSquare, castlingRights, moves);
                }
            }
        }
        return std::move(moves);
    }

    void addMoves(int x, int y, const Color activeColor, const std::optional<BoardPosition> enPassantSquare, int castlingRights,
                        std::vector<Move>& out) const {
        return newBoard.addMoves(x, y, activeColor, enPassantSquare, castlingRights, out); // TODO: deal with this, as it is invalid due to bitboards
    }

    void move(const Move& mv, const Pieces::Piece startContent, const Pieces::Piece endContent) {
        bitBoard.move(mv, startContent, endContent);
        newBoard.move(mv, startContent, endContent);
    }

    void undoMove(const Move& move, const Pieces::Piece movedPiece, const Pieces::Piece overwrittenPiece, const Color activeColor) {
        bitBoard.undoMove(move, movedPiece, overwrittenPiece, activeColor);
        newBoard.undoMove(move, movedPiece, overwrittenPiece, activeColor);
    }

    [[nodiscard]] Pieces::Piece at(const int y, const int x) const {
        return newBoard.at(y,x);
    }

    [[nodiscard]] bool isDrawFromMaterial() const {
        return newBoard.isDrawFromMaterial();
    }

    [[nodiscard]] bool isAttacked(const BoardPosition& position, Color color) const {
        return newBoard.isAttacked(position, color);
    };
    [[nodiscard]] bool isAttacked(const BoardPosition& position) const {
        return newBoard.isAttacked(position);
    };

    [[nodiscard]] const auto& get_row(const size_t i) const {
        return newBoard.get_row(i);
    }

    bool operator==(const Board& other) const {
        return newBoard == other.newBoard && bitBoard == other.bitBoard;
    }
    bool operator!=(const Board& other) const {
        return !operator==(other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Board& b) {
        os << b.newBoard;
        return os;
    }

    [[nodiscard]] NewBoard& getBoard() {
        return newBoard;
    }

private:
    NewBoard newBoard;
    BitBoardManager bitBoard;
};

/**
 * @return A board object in the starting position
 */
inline Board getStartingBoard() {
    using namespace Pieces;
    return Board({{{{BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
                      {{BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}},
                      {{WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN}},
                      {{WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}}});
}

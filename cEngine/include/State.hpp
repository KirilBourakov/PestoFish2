//
// Created by Kiril on 2025-08-31.
//
#pragma once
#include <optional>
#include <vector>

#include "ModuleOnly/CastleUtils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include "NewBoard.hpp"
#include "ZobristHash.hpp"

class State {
public:
    struct HistoricalEntry {
        Move move;
        Pieces::Piece movedPiece;       // piece moved by move
        Pieces::Piece overwrittenPiece; // piece replaced by movedPiece

        int castlingBeforeMove;
        int halfMoveClockBeforeMove;
        std::optional<BoardPosition> enPassantBeforeMove;

        bool operator==(const HistoricalEntry& other) const {
            return other.move == move && other.movedPiece == movedPiece && other.overwrittenPiece == overwrittenPiece &&
                   castlingBeforeMove == other.castlingBeforeMove && halfMoveClockBeforeMove == other.halfMoveClockBeforeMove &&
                   enPassantBeforeMove == other.enPassantBeforeMove;
        }
        bool operator!=(const HistoricalEntry& other) const {
            return !(*this == other);
        }
    };

    State();
    State(const NewBoard& board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare);
    State(const NewBoard& board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare,
          int halfMoveClock, int fullMoveClock, BoardPosition whiteKingSquare, BoardPosition blackKingSquare,
          const std::vector<HistoricalEntry>& history, const std::vector<u64>& hashHistory);

    void makeMove(const Move& move);
    void undoMove();
    std::vector<Move> getMoves();
    bool isLegalMove(BoardPosition start, BoardPosition end);
    void translateAndMove(BoardPosition start, BoardPosition end, std::optional<Pieces::Piece> promotedTo);
    GameState getGameState(const std::vector<Move>& possibleMoves);
    GameState getGameState();

    void printState() {
        board.print_board();
        if (enPassantSquare.has_value()) {
            std::cout << "en passent " << enPassantSquare.value() << std::endl;
        }
        std::cout << "Active Color " << static_cast<int>(activeColor) << std::endl;
        std::cout << "Castle Rights " << castlingRights << std::endl;
    }

    [[nodiscard]] State makeThreadCopy();
    [[nodiscard]] bool samePosition(const State& other) const;
    [[nodiscard]] bool isHalfMoveTie() const {
        return halfMoveClock >= 50;
    }
    [[nodiscard]] bool colorInCheck(const Color color) const {
        const BoardPosition kingSquare = color == Color::White ? whiteKingSquare : blackKingSquare;
        return board.isAttacked(kingSquare);
    }

    [[nodiscard]] int getCastlingRights() const {
        return castlingRights;
    }
    [[nodiscard]] u64 getZobrist() const {
        return hash.getValue();
    }
    [[nodiscard]] const NewBoard& getBoard() const {
        return board;
    }
    [[nodiscard]] const Pieces::Piece getAt(const BoardPosition inp) const {
        return board(inp.y, inp.x);
    }

    [[nodiscard]] std::optional<BoardPosition> getEnPassantSquare() const {
        return enPassantSquare;
    }
    [[nodiscard]] int getHalfMoveClock() const {
        return halfMoveClock;
    }
    [[nodiscard]] int getFullMoveClock() const {
        return fullMoveClock;
    }
    [[nodiscard]] Color getActiveColor() const {
        return activeColor;
    }

    friend bool operator==(const State& lhs, const State& rhs);
    bool operator!=(const State& other) const {
        return !(*this == other);
    }

private:
    std::vector<Move> purgeIllegal(const std::vector<Move>& pseudolegalMoves);

    // FEN info
    NewBoard board;
    Color activeColor;
    int castlingRights;
    std::optional<BoardPosition> enPassantSquare;
    int halfMoveClock;
    int fullMoveClock;

    // Other useful info
    BoardPosition whiteKingSquare;
    BoardPosition blackKingSquare;

    std::vector<HistoricalEntry> history;
    std::vector<u64> hashHistory;

    ZobristHash hash;
};

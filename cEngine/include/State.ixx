//
// Created by Kiril on 2025-08-31.
//

export module State;
import Types;
import <optional>;
import <vector>;


export class State {
public:
    State();
    State(const BoardArray &board, Color activeColor, int castlingRights, std::optional<BoardPosition> enPassantSquare);

    void makeMove(Move move);
    void undoMove();
    std::vector<Move> getMoves();

    [[nodiscard]] bool samePosition(const State& other) const;
    [[nodiscard]] int getCastlingRights() const {return castlingRights;}
    [[nodiscard]] BoardArray getBoard() const {return board;}

    friend bool operator==(const State& lhs, const State& rhs);

private:
    struct HistoricalEntry {
        Move move;
        Piece movedPiece; // piece moved by move
        Piece overwrittenPiece; // piece replaced by movedPiece

        int castlingBeforeMove;
        int halfMoveClockBeforeMove;
        std::optional<BoardPosition> enPassantBeforeMove;

        bool operator==(const HistoricalEntry& other) const {
            return other.move == move && other.movedPiece == movedPiece && other.overwrittenPiece == overwrittenPiece
                && castlingBeforeMove == other.castlingBeforeMove && halfMoveClockBeforeMove == other.halfMoveClockBeforeMove && enPassantBeforeMove == other.enPassantBeforeMove;
        }
    };

    // FEN info
    BoardArray board;
    Color activeColor;
    int castlingRights;
    std::optional<BoardPosition> enPassantSquare;
    int halfMoveClock;
    int fullMoveClock;

    // Other useful info
    BoardPosition whiteKingSquare;
    BoardPosition blackKingSquare;

    std::vector<HistoricalEntry> history;
};

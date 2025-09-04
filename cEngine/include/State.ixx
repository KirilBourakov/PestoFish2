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

    void makeMove(Move move);
    void undoMove();
    std::vector<Move> getMoves();

private:
    struct HistoricalEntry {
        Move move;
        Piece movedPiece; // piece moved by move
        Piece overwrittenPiece; // piece replaced by movedPiece

        int castlingBeforeMove;
        int halfMoveClockBeforeMove;
        std::optional<BoardPosition> enPassantBeforeMove;

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

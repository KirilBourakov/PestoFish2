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

private:
    struct HistoricalEntry {
        Move move;
        int castlingBeforeMove;
        std::optional<BoardPosition> enPassantBeforeMove;

    };

    // FEN info
    BoardArray board;
    Color activeColor;
    int castlingRights;
    std::optional<BoardPosition> enPassantSquare;
    int halfMoveClock;
    int fullMoveCock;

    // Other useful info
    BoardPosition whiteKingSquare;
    BoardPosition blackKingSquare;

    std::vector<HistoricalEntry> history;
};

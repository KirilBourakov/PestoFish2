//
// Created by Kiril on 2025-08-27.
//

export module Engine;
import <optional>;
import Board;
import Types;


export class Engine {
public:
    Engine();
    int add(int a, int b);

    void makeMove(Move move);

private:
    BoardArray board;

    Color moveTurn;
    int castlingRights;
    std::optional<BoardPosition> enPassantSquare;
    int halfMoveClock;
    int fullMoveCock;

    BoardPosition whiteKingSquare;
    BoardPosition blackKingSquare;
};

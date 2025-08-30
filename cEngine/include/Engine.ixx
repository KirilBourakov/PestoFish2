//
// Created by Kiril on 2025-08-27.
//

export module Engine;
import <optional>;
import <vector>;
import Board;
import Types;



export class Engine {
public:
    Engine();
    int add(int a, int b);

    void makeMove(Move move);

    std::vector<Move> getMoves(const BoardArray& board,  Color color, int castleRights, const std::optional<BoardPosition>& enPassantSquare);

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

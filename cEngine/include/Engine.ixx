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

    std::vector<Move> getMoves(
        BoardArray &board,
        Color color,
        int castleRights,
        BoardPosition kingPosition,
        const std::optional<BoardPosition> &enPassantSquare
    );

private:
    BoardArray board;

    Color moveTurn;
    int castlingRights;
    std::optional<BoardPosition> enPassantSquare;
    int halfMoveClock;
    int fullMoveCock;

    BoardPosition whiteKingSquare;
    BoardPosition blackKingSquare;

    void simulateMove(BoardArray& board, const Move &move);
    void undoSimulateMove(BoardArray& board, const Move &move, const Piece moved, const Piece captured, std::optional<Piece> enPassantPiece);
};

//
// Created by Kiril on 2025-08-23.
//

#include "Engine.h"
Engine::Engine() {
    board = get_start_board();

    moveTurn = WHITE;
    castlingRights = 0b1111;
    enPassantSquare = 0x88;
    halfMoveClock = 0;
    fullMoveCock = 0;

    whiteKingSquare = 0x74;
    blackKingSquare = 0x04;

}

void Engine::makeMove(Move move) {
    // TODO: handle castling, enpassent
    Piece newPiece = move.promotion == EMPTY ? board[move.start.y][move.start.x] : move.promotion;
    board[move.end.y][move.end.x] = newPiece;
    board[move.start.y][move.start.x] = EMPTY;
}

int Engine::add(int a, int b) {
    return a + b;
}


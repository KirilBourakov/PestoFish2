//
// Created by Kiril on 2025-08-23.
//

#include "Engine.h"
Engine::Engine() {
    board = get_start_board();

    moveTurn = WHITE;
    castlingRights = 0b1111;
    enPassantSquare = std::nullopt;
    halfMoveClock = 0;
    fullMoveCock = 0;

    whiteKingSquare = {.x=4, .y=7};
    blackKingSquare = {.x=4, .y=0};

}

void Engine::makeMove(Move move) {
    // TODO: handle castling, enpassent
    Piece newPiece = move.promotion.value_or(board[move.start.y][move.start.x]);
    board[move.end.y][move.end.x] = newPiece;
    board[move.start.y][move.start.x] = EMPTY;
}

int Engine::add(int a, int b) {
    return a + b;
}


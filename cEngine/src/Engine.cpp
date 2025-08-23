//
// Created by Kiril on 2025-08-23.
//

#include "Engine.h"
Engine::Engine() {
    board = get_start_board();

    castlingRights = 0b1111;
    enPassantSquare = 0x88;
    halfMoveClock = 0;
    fullMoveCock = 0;

    whiteKingSquare = 0x74;
    blackKingSquare = 0x04;

}

void Engine::makeMove(boardPostion original, boardPostion destination) {
    int oldY = getY(original);
    int oldX = getX(original);
    int newY = getY(destination);
    int newX = getX(destination);

    board[newY][newX] = board[oldY][oldX];
    board[oldY][oldX] = EMPTY;
}

int Engine::add(int a, int b) {
    return a + b;
}


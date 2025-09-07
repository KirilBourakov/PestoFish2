//
// Created by Kiril on 2025-08-23.
//
module;
#include <stdexcept>

module Engine;
import Move;

Engine::Engine() {
    board = getStartingBoard();

    moveTurn = WHITE;
    castlingRights = 0b1111;
    enPassantSquare = std::nullopt;
    halfMoveClock = 0;
    fullMoveCock = 0;

    whiteKingSquare = {.x=4, .y=7};
    blackKingSquare = {.x=4, .y=0};

}

int Engine::add(int a, int b) {
    return a + b;
}


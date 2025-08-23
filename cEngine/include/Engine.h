//
// Created by Kiril on 2025-08-23.
//
#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#include "Board.h"


class Engine {
public:
    Engine();
    int add(int a, int b);

    void makeMove(Move move);

private:
    BoardArray board;

    Color moveTurn;
    int castlingRights;
    boardPostion enPassantSquare;
    int halfMoveClock;
    int fullMoveCock;

    boardPostion whiteKingSquare;
    boardPostion blackKingSquare;
};



#endif //ENGINE_H

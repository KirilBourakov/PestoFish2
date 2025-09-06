//
// Created by Kiril on 2025-09-06.
//
#include "gtest/gtest.h"
import State;
import testBoard;
import Types;

TEST(getMoves, whiteCheckmated) {
    State state{
        whiteBackRanked(),
        WHITE,
        0b1111,
        std::nullopt
    };

    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(0, moves.size());
}

TEST(getMoves, whiteSavedByBishop) {
    State state{
        whiteSavedByBishop(),
        WHITE,
        0b1111,
        std::nullopt
    };
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(1, moves.size());
}


TEST(MakeUndo, pawnTakesPawn) {
    State state{
        foolsMatePosition(),
        WHITE,
        0b1111,
        std::nullopt
    };
    State stateCopy{state};

    state.makeMove(Move::standardMove({2, 4}, {1,3}));
    ASSERT_EQ(0b1111, state.getCastlingRights());
    ASSERT_EQ(WHITE_PAWN, state.getBoard()[3][1]);
    ASSERT_EQ(EMPTY, state.getBoard()[4][2]);

    state.undoMove();
    ASSERT_EQ(stateCopy, state);
}

TEST(MakeUndo, whiteSavedByBishop) {
    State state{
        whiteSavedByBishop(),
        WHITE,
        0b1111,
        std::nullopt
    };
    State stateCopy{state};
    std::vector<Move> moves = state.getMoves();
    state.makeMove(moves.at(0));
    ASSERT_EQ(0b1111, state.getCastlingRights());
    ASSERT_EQ(EMPTY, state.getBoard()[6][4]);
    ASSERT_EQ(WHITE_BISHOP, state.getBoard()[7][3]);
    state.undoMove();
    ASSERT_EQ(stateCopy, state);
}
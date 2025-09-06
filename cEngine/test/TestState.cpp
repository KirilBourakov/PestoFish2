//
// Created by Kiril on 2025-09-06.
//
#include "gtest/gtest.h"
import State;
import testBoard;
import Types;

TEST(Test, pawnTakesPawn) {
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

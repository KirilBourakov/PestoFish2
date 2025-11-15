//
// Created by Kiril on 2025-11-15.
//

#include "ModuleOnly/parse.hpp"
#include "State.hpp"
#include "testBoard.hpp"
#include "gtest/gtest.h"

TEST(prase, fenInitial) {
    State state;
    State from = fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    ASSERT_EQ(state, from);
}

TEST(prase, fenUnBlockedCastle) {
    State state = {NewBoard(unBlockedCastle()), Color::Black, 0, std::nullopt, 5, 25};
    State from = fenToState("r3k2r/8/8/8/8/8/3PPP2/R3K2R b - - 5 25");
}

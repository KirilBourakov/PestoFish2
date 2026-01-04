//
// Created by Kiril on 1/4/2026.
//
#include "Nnue.hpp"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "ModuleOnly/parse.hpp"

TEST(NNUE, startPosMoveUndo) {
    State state = fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::optional<Nnue> nnue = Nnue();

    int startValue1 = nnue.value().setBoard(state.getBoard(), state.getActiveColor());

    state.makeMove(Move::standardMove({.x=4, .y=6}, {.x=4, .y=5}), nnue);
    int interValue1 = nnue.value().eval(state.getActiveColor());
    state.undoMove(nnue);

    int startValue2 = nnue.value().setBoard(state.getBoard(), state.getActiveColor());

    state.makeMove(Move::standardMove({.x=4, .y=6}, {.x=4, .y=5}), nnue);
    int interValue2 = nnue.value().eval(state.getActiveColor());

    EXPECT_EQ(startValue1, startValue2);
    EXPECT_EQ(interValue1, interValue2);
}

TEST(NNUE, startPosArrival) {
    State state1 = fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    std::optional<Nnue> nnue1 = Nnue();
    nnue1.value().setBoard(state1.getBoard(), state1.getActiveColor());
    state1.makeMove(Move::standardMove({.x=4, .y=6}, {.x=4, .y=5}), nnue1);

    int arrivedValue = nnue1.value().eval(state1.getActiveColor());

    State state2 = fenToState("rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    std::optional<Nnue> nnue2 = Nnue();
    nnue2.value().setBoard(state2.getBoard(), state2.getActiveColor());

    int expectedValue = nnue2.value().eval(state2.getActiveColor());
    EXPECT_EQ(expectedValue, arrivedValue);
}
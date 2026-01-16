//
// Created by Kiril on 2025-10-04.
//
#include "Engine.hpp"
#include "gtest/gtest.h"

#include "ModuleOnly/Utils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include "Preft.hpp"
#include "gmock/gmock-matchers.h"
#include "ModuleOnly/parse.hpp"

using namespace Pieces;

TEST(TestEngine, testSimpleGetBest) {
    Engine engine{};
    engine.setState(fenToState("r3k2r/8/8/8/8/6P1/P3PPP1/R3K2R w - - 0 1"));
    engine.updatedMainNnue();

    const Move best = engine.getBestMove();

    engine.setState(fenToState("r3k2R/8/8/8/8/6P1/P3PPP1/R3K3 b - - 0 1"));
    engine.updatedMainNnue();

    ASSERT_EQ(best, Move::standardMove(BoardPosition{7, 7}, BoardPosition{7, 0}));
}

TEST(TestEngine, testSmotherMateInOne) {
    Engine engine{};
    engine.setState(fenToState(("6rk/6pp/7N/8/8/8/8/7K w - - 0 1")));
    engine.updatedMainNnue();

    const Move best = engine.getBestMove();
    ASSERT_EQ(best, Move::standardMove(BoardPosition{7, 2}, BoardPosition{5, 1}));
}

TEST(TestEngine, BackRank1) {
    Engine engine{};
    engine.setState(fenToState(("6k1/5ppp/8/8/8/8/8/4R1K1 w - - 0 1")));
    engine.updatedMainNnue();

    const Move best = engine.getBestMove();
    ASSERT_EQ(best, Move::standardMove(BoardPosition{4, 7}, BoardPosition{4, 0}));
}

TEST(TestEngine, BackRank2) {
    Engine engine{};
    engine.setState(fenToState(("6k1/5ppp/8/8/4r3/8/8/R6K w - - 0 1")));
    engine.updatedMainNnue();

    const Move best1 = engine.getBestMove();
    engine.getState().makeMove(best1, nullptr);
    const Move best2 = engine.getBestMove();
    engine.getState().makeMove(best2, nullptr);
    const Move best3 = engine.getBestMove();


    ASSERT_EQ(best1, Move::standardMove(BoardPosition{0, 7}, BoardPosition{0, 0}));
    ASSERT_EQ(best2, Move::standardMove(BoardPosition{4, 4}, BoardPosition{4, 0}));
    ASSERT_EQ(best3, Move::standardMove(BoardPosition{0, 0}, BoardPosition{4, 0}));
}

TEST(TestEngine, queenMate1) {
    Engine engine{};
    engine.setState(fenToState(("7k/5Q2/6K1/8/8/8/8/8 w - - 0 1")));
    engine.updatedMainNnue();

    SearchRequest req;
    req.depth = 1;
    const Move best = engine.getBestMove(req);

    constexpr auto queenPos = BoardPosition{5, 1};
    EXPECT_THAT(best, testing::AnyOf(
        Move::standardMove(queenPos,BoardPosition{6, 1}),
        Move::standardMove(queenPos,BoardPosition{7, 1}),
        Move::standardMove(queenPos,BoardPosition{5, 0}),
        Move::standardMove(queenPos,BoardPosition{4, 0})
    ));
}


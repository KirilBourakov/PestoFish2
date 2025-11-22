//
// Created by Kiril on 2025-10-04.
//
#include "Engine.hpp"
#include "gtest/gtest.h"

#include "ModuleOnly/CastleUtils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include "testBoard.hpp"
#include "ModuleOnly/parse.hpp"

using namespace Pieces;

TEST(TestEngine, testSimpleGetBest) {
    Engine engine{};
    engine.setState(State{simpleBest(), Color::White, 0b1111, std::nullopt});

    const Move best = engine.getBestMove();
    ASSERT_EQ(best, Move::standardMove(BoardPosition{7, 7}, BoardPosition{7, 0}));
}

TEST(TestEngine, testSmotherMateInOne) {
    Engine engine{};
    engine.setState(fenToState(("6rk/6pp/7N/8/8/8/8/7K w - - 0 1")));

    const Move best = engine.getBestMove();
    ASSERT_EQ(best, Move::standardMove(BoardPosition{7, 2}, BoardPosition{5, 1}));
}

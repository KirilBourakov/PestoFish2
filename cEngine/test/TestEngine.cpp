//
// Created by Kiril on 2025-10-04.
//
#include "Engine.hpp"
#include "gtest/gtest.h"

#include "ModuleOnly/Board.hpp"
#include "ModuleOnly/Enums.hpp"
#include "Move.hpp"
#include "testBoard.hpp"

using namespace Pieces;

TEST(TestEngine, testSimpleGetBest) {
    Engine engine{};
    engine.setState(State{simpleBest(), Color::White, 0b1111, std::nullopt});

    const Move best = engine.getBestMove();
    ASSERT_EQ(best, Move::standardMove(BoardPosition{7, 7}, BoardPosition{7, 0}));
}

TEST(TestEngine, testSmotherMateInOne) {
    Engine engine{};
    engine.setState(State{smotheredMatePosition(), Color::White, 0b1111, std::nullopt});

    const Move best = engine.getBestMove();
    ASSERT_EQ(best, Move::standardMove(BoardPosition{4, 4}, BoardPosition{3, 2}));
}

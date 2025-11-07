//
// Created by Kiril on 2025-10-04.
//
#include "gtest/gtest.h"

import Engine;
import testBoard;
import State;
import Move;
import Enums;
import Board;

using namespace Pieces;

TEST(TestEngine, testSimpleGetBest)
{
  Engine engine{};
  engine.setState(State{ simpleBest(), Color::White, 0b1111, std::nullopt });

  const Move best = engine.getBestMove();
  ASSERT_EQ(best,
            Move::standardMove(BoardPosition{ 7, 7 }, BoardPosition{ 7, 0 }));
}

TEST(TestEngine, testSmotherMateInOne)
{
  Engine engine{};
  engine.setState(
    State{ smotheredMatePosition(), Color::White, 0b1111, std::nullopt });

  const Move best = engine.getBestMove();
  ASSERT_EQ(best,
            Move::standardMove(BoardPosition{ 4, 4 }, BoardPosition{ 3, 2 }));
}

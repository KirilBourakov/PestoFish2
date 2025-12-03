#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Board/ArrayBoard.hpp"
#include "testBoard.hpp"

#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"

// KING IN CHECK
TEST(TestMove, NoCheck) {
    EXPECT_FALSE(randomMiddleGame().inCheck(Color::Black));
    EXPECT_FALSE(randomMiddleGame().inCheck(Color::White));
}

TEST(TestMove, CheckBlockedByPawn) {
    EXPECT_FALSE(randomMiddleGame().inCheck(Color::White));
}

TEST(TestMove, KnightCheck) {
    EXPECT_TRUE(knightCheck().inCheck(Color::White));
    EXPECT_TRUE(knightCheck().inCheck(Color::Black));
}

TEST(TestMove, StrightCheck) {
    EXPECT_TRUE(straightCheck().inCheck(Color::White));
    EXPECT_TRUE(straightCheck().inCheck(Color::Black));
}

TEST(TestMove, DiagCheck) {
    EXPECT_TRUE(diagCheck().inCheck(Color::White));
    EXPECT_TRUE(diagCheck().inCheck(Color::Black));
}

TEST(TestMove, PawnCheck) {
    EXPECT_TRUE(pawnCheck().inCheck(Color::Black));
    EXPECT_TRUE(pawnCheck().inCheck(Color::White));
}

TEST(TestMove, PawnBackNotCheck) {
    EXPECT_FALSE(kingsFlankPawns().inCheck(Color::Black));
    EXPECT_FALSE(kingsFlankPawns().inCheck(Color::White));
}

TEST(TestMove, KingCheck) {
    EXPECT_TRUE(kingCheck().inCheck(Color::Black));
    EXPECT_TRUE(kingCheck().inCheck(Color::White));
}
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Board/ArrayBoard.hpp"
#include "Preft.hpp"

#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include "ModuleOnly/parse.hpp"

// KING IN CHECK
TEST(TestMove, NoCheck) {
    EXPECT_FALSE(fenToState("rnb1kbnr/p1pp1ppp/4q3/1p4pP/2P5/3P4/PP2PPPP/RNBQKBNR w - - 0 1").getBoard().inCheck(Color::Black));
    EXPECT_FALSE(fenToState("rnb1kbnr/p1pp1ppp/4q3/1p4pP/2P5/3P4/PP2PPPP/RNBQKBNR w - - 0 1").getBoard().inCheck(Color::White));
}

TEST(TestMove, CheckBlockedByPawn) {
    EXPECT_FALSE(fenToState("rnb1kbnr/p1pp1ppp/4q3/1p4pP/2P5/3P4/PP2PPPP/RNBQKBNR w - - 0 1").getBoard().inCheck(Color::White));
}

TEST(TestMove, KnightCheck) {
    EXPECT_TRUE(fenToState("4k3/7r/4Q1N1/4P3/6R1/3n4/P7/3QKB2 w - - 0 1").getBoard().inCheck(Color::White));
    EXPECT_TRUE(fenToState("4k3/7r/4Q1N1/4P3/6R1/3n4/P7/3QKB2 w - - 0 1").getBoard().inCheck(Color::Black));
}

TEST(TestMove, StrightCheck) {
    EXPECT_TRUE(fenToState("4k2R/7r/4q3/4P3/8/8/P7/2q1K3 w - - 0 1").getBoard().inCheck(Color::White));
    EXPECT_TRUE(fenToState("4k2R/7r/4q3/4P3/8/8/P7/2q1K3 w - - 0 1").getBoard().inCheck(Color::Black));
}

TEST(TestMove, DiagCheck) {
    EXPECT_TRUE(fenToState("4k3/7r/4q3/4P3/B7/1q6/P7/3K4 w - - 0 1").getBoard().inCheck(Color::White));
    EXPECT_TRUE(fenToState("4k3/7r/4q3/4P3/B7/1q6/P7/3K4 w - - 0 1").getBoard().inCheck(Color::Black));
}

TEST(TestMove, PawnCheck) {
    EXPECT_TRUE(fenToState("1k6/P7/2p5/1K6/8/8/8/8 w - - 0 1").getBoard().inCheck(Color::Black));
    EXPECT_TRUE(fenToState("1k6/P7/2p5/1K6/8/8/8/8 w - - 0 1").getBoard().inCheck(Color::White));
}

TEST(TestMove, PawnBackNotCheck) {
    const State s = fenToState("8/3K4/1kp5/8/8/8/8/8 w - - 0 1");
    EXPECT_FALSE(s.getBoard().inCheck(Color::Black));
    EXPECT_FALSE(s.getBoard().inCheck(Color::White));
}

TEST(TestMove, KingCheck) {
    EXPECT_TRUE(fenToState("1k6/K7/8/8/8/8/8/8 w - - 0 1").getBoard().inCheck(Color::Black));
    EXPECT_TRUE(fenToState("1k6/K7/8/8/8/8/8/8 w - - 0 1").getBoard().inCheck(Color::White));
}
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ArrayBoard.hpp"
#include "testBoard.hpp"

#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"

using ::testing::UnorderedElementsAreArray;
using namespace Pieces;

// KING IN CHECK
TEST(TestMove, NoCheck) {
    EXPECT_FALSE(randomMiddleGame().isAttacked(BoardPosition{4, 7}));
    EXPECT_FALSE(randomMiddleGame().isAttacked(BoardPosition{4, 0}));
}

TEST(TestMove, CheckBlockedByPawn) {
    EXPECT_FALSE(randomMiddleGame().isAttacked(BoardPosition{4, 7}));
}

TEST(TestMove, KnightCheck) {
    EXPECT_TRUE(knightCheck().isAttacked(BoardPosition{4, 7}));
    EXPECT_TRUE(knightCheck().isAttacked(BoardPosition{4, 0}));
}

TEST(TestMove, StrightCheck) {
    EXPECT_TRUE(straightCheck().isAttacked(BoardPosition{4, 7}));
    EXPECT_TRUE(straightCheck().isAttacked(BoardPosition{4, 0}));
}

TEST(TestMove, DiagCheck) {
    EXPECT_TRUE(diagCheck().isAttacked(BoardPosition{3, 7}));
    EXPECT_TRUE(diagCheck().isAttacked(BoardPosition{4, 0}));
}

TEST(TestMove, PawnCheck) {
    EXPECT_TRUE(pawnCheck().isAttacked(BoardPosition{1, 0}));
    EXPECT_TRUE(pawnCheck().isAttacked(BoardPosition{1, 3}));
}

TEST(TestMove, PawnBackNotCheck) {
    EXPECT_FALSE(kingsFlankPawns().isAttacked(BoardPosition{1, 2}));
    EXPECT_FALSE(kingsFlankPawns().isAttacked(BoardPosition{3, 1}));
}

TEST(TestMove, InvalidPostion) {
    EXPECT_THROW((void)pawnCheck().isAttacked(BoardPosition{9, 9}), std::invalid_argument);
}

TEST(TestMove, KingCheck) {
    EXPECT_TRUE(kingCheck().isAttacked(BoardPosition{1, 0}));
    EXPECT_TRUE(kingCheck().isAttacked(BoardPosition{0, 1}));
}

TEST(TestMove, notProtected) {
    EXPECT_FALSE(pawnCheck().isAttacked(BoardPosition{7, 7}, Color::Black));
}

// Sliding Moves
TEST(TestMove, RookSlidingMoves) {
    std::vector<Move> moves;
    unBlockedCastle().getBoard().addSlidingMoves(7, 7, Color::White, true, false, moves);

    constexpr BoardPosition start{7, 7};
    const std::vector<Move> expectedMoves = {Move::standardMove(start, {6, 7}), Move::standardMove(start, {5, 7}),

                                             Move::standardMove(start, {7, 6}), Move::standardMove(start, {7, 5}),
                                             Move::standardMove(start, {7, 4}), Move::standardMove(start, {7, 3}),
                                             Move::standardMove(start, {7, 2}), Move::standardMove(start, {7, 1}),
                                             Move::standardMove(start, {7, 0})};
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, BishopSlidingMoves) {
    std::vector<Move> moves;
    smotheredMatePosition().getBoard().addSlidingMoves(3, 0, Color::Black, false, true, moves);

    constexpr BoardPosition start{3, 0};
    const std::vector<Move> expectedMoves = {Move::standardMove(start, {2, 1}), Move::standardMove(start, {1, 2}),
                                             Move::standardMove(start, {0, 3})};
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

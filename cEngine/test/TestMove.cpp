#include <gtest/gtest.h>

import Move;
import Types;
import testBoard;

TEST(TestMove, GetMovesFromStartWhitePawn) {
    std::vector<Move> moves;
    addPawnMoves(foolsMatePosition(), 0, 6, WHITE, std::nullopt, moves);


    constexpr BoardPosition start{.x = 0, .y = 6};
    const std::vector<Move> expectedMoves = {
        {start, BoardPosition{0, 5}},
        {start, BoardPosition{0, 4}}
    };

    ASSERT_EQ(expectedMoves, moves);
}

TEST(TestMove, GetMovesFromStartBlackPawn) {
    std::vector<Move> moves;
    addPawnMoves(foolsMatePosition(), 5, 1, BLACK, std::nullopt, moves);


    constexpr BoardPosition start{.x = 5, .y = 1};
    const std::vector<Move> expectedMoves = {
        {start, BoardPosition{5, 2}},
        {start, BoardPosition{5, 3}}
    };

    ASSERT_EQ(expectedMoves, moves);
}


TEST(TestMove, GetMovesWhitePawnCaptureAndPush) {
    std::vector<Move> moves;
    addPawnMoves(foolsMatePosition(), 2, 4, WHITE, std::nullopt, moves);

    constexpr BoardPosition start{.x = 2, .y = 4};
    const std::vector<Move> expectedMoves = {
        {start, BoardPosition{2, 3}},
        {start, BoardPosition{1, 3}}
    };

    ASSERT_EQ(expectedMoves, moves);
}

TEST(TestMove, BlackPawnBlocked) {
    std::vector<Move> moves;
    addPawnMoves(foolsMatePosition(), 7, 1, BLACK, std::nullopt, moves);

    constexpr BoardPosition start{.x = 7, .y = 1};
    const std::vector<Move> expectedMoves = {
        {start, BoardPosition{7, 2}}
    };

    ASSERT_EQ(expectedMoves, moves);
}
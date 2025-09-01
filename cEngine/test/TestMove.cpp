#include <gtest/gtest.h>
#include <gmock/gmock.h>
using ::testing::UnorderedElementsAreArray;


import Move;
import Types;
import testBoard;


// KING IN CHECK
TEST(TestMove, NoCheck) {
    EXPECT_FALSE(isAttacked(foolsMatePosition(), BoardPosition{4,7}));
    EXPECT_FALSE(isAttacked(foolsMatePosition(), BoardPosition{4,0}));
}

TEST(TestMove, CheckBlockedByPawn) {
    EXPECT_FALSE(isAttacked(foolsMatePosition(), BoardPosition{4,7}));
}

TEST(TestMove, KnightCheck) {
    EXPECT_TRUE(isAttacked(knightCheck(), BoardPosition{4,7}));
    EXPECT_TRUE(isAttacked(knightCheck(), BoardPosition{4,0}));
}

TEST(TestMove, StrightCheck) {
    EXPECT_TRUE(isAttacked(straightCheck(), BoardPosition{4,7}));
    EXPECT_TRUE(isAttacked(straightCheck(), BoardPosition{4,0}));
}

TEST(TestMove, DiagCheck) {
    EXPECT_TRUE(isAttacked(diagCheck(), BoardPosition{3,7}));
    EXPECT_TRUE(isAttacked(diagCheck(), BoardPosition{4,0}));
}

TEST(TestMove, PawnCheck) {
    EXPECT_TRUE(isAttacked(pawnCheck(), BoardPosition{1,0}));
    EXPECT_TRUE(isAttacked(pawnCheck(), BoardPosition{1,3}));
}

TEST(TestMove, PawnBackNotCheck) {
    EXPECT_FALSE(isAttacked(kingsFlankPawns(), BoardPosition{1,2}));
    EXPECT_FALSE(isAttacked(kingsFlankPawns(), BoardPosition{3,1}));
}

TEST(TestMove, InvalidPostion) {
    EXPECT_THROW(isAttacked(pawnCheck(), BoardPosition{9,9}), std::invalid_argument);
}

TEST(TestMove, KingCheck) {
    EXPECT_TRUE(isAttacked(kingCheck(), BoardPosition{1,0}));
    EXPECT_TRUE(isAttacked(kingCheck(), BoardPosition{0,1}));
}

TEST(TestMove, notPortected) {
    EXPECT_FALSE(isAttacked(pawnCheck(), BoardPosition{7,7}));
}

// PAWN MOVES
TEST(TestMove, GetMovesFromStartWhitePawn) {
    std::vector<Move> moves;
    addPawnMoves(foolsMatePosition(), 0, 6, WHITE, std::nullopt, moves);


    constexpr BoardPosition start{.x = 0, .y = 6};
    const std::vector<Move> expectedMoves = {
        {start, BoardPosition{0, 5}},
        {.start = start, .end = BoardPosition{0, 4}, .enPassant = BoardPosition{0, 5}},
    };

    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, GetMovesFromStartBlackPawn) {
    std::vector<Move> moves;
    addPawnMoves(foolsMatePosition(), 5, 1, BLACK, std::nullopt, moves);


    constexpr BoardPosition start{.x = 5, .y = 1};
    const std::vector<Move> expectedMoves = {
        {start, BoardPosition{5, 2}},
        {.start = start, .end = BoardPosition{5, 3}, .enPassant = BoardPosition{5, 2}}
    };

    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}


TEST(TestMove, GetMovesWhitePawnCaptureAndPush) {
    std::vector<Move> moves;
    addPawnMoves(foolsMatePosition(), 2, 4, WHITE, std::nullopt, moves);

    constexpr BoardPosition start{.x = 2, .y = 4};
    const std::vector<Move> expectedMoves = {
        {start, BoardPosition{2, 3}, EMPTY},
        {start, BoardPosition{1, 3}, BLACK_PAWN}
    };

    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, BlackPawnBlocked) {
    std::vector<Move> moves;
    addPawnMoves(foolsMatePosition(), 7, 1, BLACK, std::nullopt, moves);

    constexpr BoardPosition start{.x = 7, .y = 1};
    const std::vector<Move> expectedMoves = {
        {start, BoardPosition{7, 2}}
    };

    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

// Knight Moves
TEST(TestMove, smotheredMateKnight) {
    std::vector<Move> moves;
    addKnightMoves(smotheredMatePosition(), 4, 4, WHITE, moves);

    constexpr BoardPosition start{ 4, 4};
    const std::vector<Move> expectedMoves = {
        {start, {5, 2}, EMPTY},
        {start, {3, 2}, EMPTY},

        {start, {6, 3}, EMPTY},
        {start, {2, 5}, EMPTY},
        {start, {2, 3}, BLACK_ROOK},
    };

    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, cornerKnight) {
    std::vector<Move> moves;
    addKnightMoves(smotheredMatePosition(), 0, 5, WHITE, moves);
    constexpr BoardPosition start{ 0, 5};
    const std::vector<Move> expectedMoves = {
        {start, {1, 3}, EMPTY},
        {start, {2, 4}, EMPTY},
        {start, {1, 7}, EMPTY},
    };
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

// King Moves
TEST(TestMove, stuckKing) {
    std::vector<Move> moves;
    addKingMoves(smotheredMatePosition(), 4, 0, BLACK, 0b1111, moves);
    const std::vector<Move> expectedMoves = {};
    for (Move move : moves) {
        std::cout << move << std::endl;
    }

    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, StanderedKingMove) {
    std::vector<Move> moves;
    addKingMoves(smotheredMatePosition(), 4, 7, WHITE, 0b1111, moves);
    constexpr BoardPosition start{ 4, 7};
    const std::vector<Move> expectedMoves = {
        {start, {4, 6,}, EMPTY},
        {start, {5, 7,}, EMPTY},
        {start, {6, 7}, EMPTY,  std::nullopt, SHORT, std::nullopt},
    };
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, StanderedKingWithoutCastleRights) {
    std::vector<Move> moves;
    addKingMoves(smotheredMatePosition(), 4, 7, WHITE, 0b0111, moves);
    constexpr BoardPosition start{ 4, 7};
    const std::vector<Move> expectedMoves = {
        {start, {4,6}},
        {start, {5, 7}}
    };
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, CastleAllowed) {
    std::vector<Move> moves;
    addKingMoves(unBlockedCastle(), 4, 7, WHITE, 0b1111, moves);
    constexpr BoardPosition start{ 4, 7};
    const std::vector<Move> expectedMoves = {
        {start, {3, 7}, EMPTY},
        {start, {5, 7}, EMPTY},
        {start, {6, 7}, EMPTY, std::nullopt, SHORT},
        {start, {2, 7}, EMPTY, std::nullopt, LONG}
    };
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));

    moves = {};
    addKingMoves(unBlockedCastle(), 4, 0, BLACK, 0b1111, moves);
    constexpr BoardPosition bStart{ 4, 0};
    const std::vector<Move> bExpectedMoves = {
        {bStart, {3, 0}, EMPTY},
        {bStart, {5, 0}, EMPTY},
        {bStart, {4, 1}, EMPTY},
        {bStart, {3, 1}, EMPTY},
        {bStart, {5, 1}, EMPTY},

        {bStart, {6, 0}, EMPTY, std::nullopt, SHORT},
        {bStart, {2, 0}, EMPTY, std::nullopt, LONG}
    };
    EXPECT_THAT(bExpectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, CastleDisAllowed) {
    std::vector<Move> moves;
    addKingMoves(unBlockedCastle(), 4, 7, WHITE, 0b0000, moves);
    constexpr BoardPosition start{ 4, 7};
    const std::vector<Move> expectedMoves = {
        {start, {3, 7}},
        {start, {5, 7}},
    };
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));

    moves = {};
    addKingMoves(unBlockedCastle(), 4, 0, BLACK, 0b0010, moves);
    constexpr BoardPosition bStart{ 4, 0};
    const std::vector<Move> bExpectedMoves = {
        {bStart, {3, 0}, EMPTY},
        {bStart, {5, 0}, EMPTY},
        {bStart, {4, 1}, EMPTY},
        {bStart, {3, 1}, EMPTY},
        {bStart, {5, 1}, EMPTY},

        {bStart, {6, 0}, EMPTY, std::nullopt, SHORT},
    };
    EXPECT_THAT(bExpectedMoves, UnorderedElementsAreArray(moves));
}

// Sliding Moves
TEST(TestMove, RookSlidingMoves) {
    std::vector<Move> moves;
    addSlidingMoves(unBlockedCastle(), 7, 7, WHITE, true, false, moves);

    constexpr BoardPosition start{ 7, 7};
    const std::vector<Move> expectedMoves = {
        {start, {6, 7}, EMPTY},
        {start, {5, 7}, EMPTY},

        {start, {7, 6}, EMPTY},
        {start, {7, 5}, EMPTY},
        {start, {7, 4}, EMPTY},
        {start, {7, 3}, EMPTY},
        {start, {7, 2}, EMPTY},
        {start, {7, 1}, EMPTY},
        {start, {7, 0}, BLACK_ROOK}
    };
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}

TEST(TestMove, BishopSlidingMoves) {
    std::vector<Move> moves;
    addSlidingMoves(smotheredMatePosition(), 3, 0, BLACK, false, true, moves);

    constexpr BoardPosition start{ 3, 0};
    const std::vector<Move> expectedMoves = {
        {start, {2,1}},
        {start, {1, 2}},
        {start, {0, 3}}
    };
    EXPECT_THAT(expectedMoves, UnorderedElementsAreArray(moves));
}
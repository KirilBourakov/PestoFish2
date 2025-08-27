#include <gtest/gtest.h>

#include "Move.h"
#include "Types.h"
#include "testBoards.h"

// BoardArray foolsMatePosition() {
//     return BoardArray{{
//         {{BLACK_ROOK,   BLACK_KNIGHT, BLACK_BISHOP, EMPTY,       BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
//         {{BLACK_PAWN,   EMPTY,        BLACK_PAWN,   BLACK_PAWN,  EMPTY,      BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN}},
//         {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_QUEEN,EMPTY,        EMPTY,        EMPTY}},
//         {{EMPTY,        BLACK_PAWN,   EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        WHITE_PAWN}},
//         {{EMPTY,        EMPTY,        WHITE_PAWN,   EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
//         {{EMPTY,        EMPTY,        EMPTY,        WHITE_PAWN,  EMPTY,      EMPTY,        EMPTY,        EMPTY}},
//         {{WHITE_PAWN,   WHITE_PAWN,   EMPTY,        EMPTY,       WHITE_PAWN, WHITE_PAWN,   WHITE_PAWN,   EMPTY}},
//         {{WHITE_ROOK,   WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}
//     }};
// }

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
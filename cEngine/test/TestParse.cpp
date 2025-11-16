//
// Created by Kiril on 2025-11-15.
//

#include "Engine.hpp"
#include "ModuleOnly/parse.hpp"
#include "State.hpp"
#include "testBoard.hpp"
#include "gtest/gtest.h"

// PARSE SQUARES

TEST(praseSquare, fenInitial) {
    State state;
    State from = fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    ASSERT_EQ(state, from);
}

TEST(praseSquare, fenUnBlockedCastle) {
    State state = {NewBoard(unBlockedCastle()), Color::Black, 0, std::nullopt, 5, 25};
    State from = fenToState("r3k2r/8/8/8/8/8/3PPP2/R3K2R b - - 5 25");
}

TEST(praseSquare, SquareToPosBasic) {
    auto a1 = squareToPos("a1");
    EXPECT_EQ(a1.x, 0);
    EXPECT_EQ(a1.y, 7);

    auto h8 = squareToPos("h8");
    EXPECT_EQ(h8.x, 7);
    EXPECT_EQ(h8.y, 0);

    auto d5 = squareToPos("d5");
    EXPECT_EQ(d5.x, 3);
    EXPECT_EQ(d5.y, 3);
}

TEST(praseSquare, PosToSquareBasic) {
    EXPECT_EQ(posToSquare({0, 7}), "a1");
    EXPECT_EQ(posToSquare({7, 0}), "h8");
    EXPECT_EQ(posToSquare({3, 3}), "d5");
}

TEST(praseSquare, RoundTripSquares) {
    for (char f = 'a'; f <= 'h'; f++) {
        for (char r = '1'; r <= '8'; r++) {
            std::string sq;
            sq += f;
            sq += r;
            EXPECT_EQ(posToSquare(squareToPos(sq)), sq);
        }
    }
}

// Standered Move

TEST(parseMove, StandardMoveParsing) {
    State s = fenToState("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    auto mv = moveFromLongAlgebric("e4e5", s);

    EXPECT_EQ(mv.start.x, 4);
    EXPECT_EQ(mv.start.y, 4);
    EXPECT_EQ(mv.end.x, 4);
    EXPECT_EQ(mv.end.y, 3);
    EXPECT_FALSE(mv.promotedTo.has_value());
    EXPECT_FALSE(mv.castle.has_value());
}

// Pawn Double

TEST(parseMove, PawnDoublePush) {
    State s = fenToState("8/8/8/8/8/8/4P3/8 w - - 0 1");

    auto mv = moveFromLongAlgebric("e2e4", s);

    ASSERT_TRUE(mv.newEnPassantSquare.has_value());
    EXPECT_EQ(mv.newEnPassantSquare->x, 4);
    EXPECT_EQ(mv.newEnPassantSquare->y, 5);
}

// PROMOTION

TEST(parseMove, PromotionToQueen) {
    State s = fenToState("8/4P3/8/8/8/8/8/4k3 w - - 0 1");

    auto mv = moveFromLongAlgebric("e7e8q", s);

    ASSERT_TRUE(mv.promotedTo.has_value());
    EXPECT_EQ(Pieces::piece_type(mv.promotedTo.value()), PieceType::Queen);
}

TEST(parseMove, PromotionToKnight) {
    State s = fenToState("8/4P3/8/8/8/8/8/4k3 w - - 0 1");

    auto mv = moveFromLongAlgebric("e7e8n", s);

    ASSERT_TRUE(mv.promotedTo.has_value());
    EXPECT_EQ(Pieces::piece_type(mv.promotedTo.value()), PieceType::Knight);
}

TEST(parseMove, promoteCorner) {
    Engine engine{};
    engine.setState(fenToState("2k3r1/P7/8/8/8/8/8/7K w - - 0 1"));
    Move move = engine.getBestMove();

    auto mv = longAlgebricFromMove(move);
    EXPECT_EQ(mv, "a7a8q");
}

// CASTLE

TEST(parseMove, WhiteShortCastle) {
    State s = fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");

    auto mv = moveFromLongAlgebric("e1g1", s);

    ASSERT_TRUE(mv.castle.has_value());
    EXPECT_EQ(mv.castle.value(), CastleType::SHORT);
}

TEST(parseMove, WhiteLongCastle) {
    State s = fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w KQkq - 0 1");

    auto mv = moveFromLongAlgebric("e1c1", s);

    ASSERT_TRUE(mv.castle.has_value());
    EXPECT_EQ(mv.castle.value(), CastleType::LONG);
}

TEST(parseMove, BlackShortCastle) {
    State s = fenToState("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

    auto mv = moveFromLongAlgebric("e8g8", s);

    ASSERT_TRUE(mv.castle.has_value());
    EXPECT_EQ(mv.castle.value(), CastleType::SHORT);
}

TEST(parseMove, BlackLongCastle) {
    State s = fenToState("r3kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

    auto mv = moveFromLongAlgebric("e8c8", s);

    ASSERT_TRUE(mv.castle.has_value());
    EXPECT_EQ(mv.castle.value(), CastleType::LONG);
}

// Round Trip

TEST(parseMove, RoundTripPromotion) {
    State s = fenToState("8/4P3/8/8/8/8/8/4k3 w - - 0 1");

    auto mv = moveFromLongAlgebric("e7e8q", s);
    EXPECT_EQ(longAlgebricFromMove(mv), "e7e8q");
}

TEST(parseMove, RoundTripCastle) {
    State s = fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");

    auto mv = moveFromLongAlgebric("e1g1", s);
    EXPECT_EQ(longAlgebricFromMove(mv), "e1g1");
}

//
// Created by Kiril on 2025-09-06.
//
#include "ModuleOnly/CastleUtils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "gtest/gtest.h"

#include "ModuleOnly/Move.hpp"
#include "State.hpp"
#include "testBoard.hpp"
#include "ModuleOnly/parse.hpp"

using namespace Pieces;

// --- GET STATE --- //

TEST(GameState, Scholars) {
    State state = fenToState("r1bqk1nr/pppp1Qpp/2n5/2b1p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4");
    ASSERT_EQ(state.getGameState(), GameState::WHITE_WIN);
}

TEST(GameState, Fools) {
    State state = fenToState("rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3");
    ASSERT_EQ(state.getGameState(), GameState::BLACK_WIN);
}

TEST(GameState, Stalemate) {
    State state = fenToState("7k/5K2/6Q1/8/8/8/8/8 b - - 10 60");
    ASSERT_EQ(state.getGameState(), GameState::STALEMATE);
}

TEST(GameState, InsufficantMaterial) {
    State state = fenToState("8/8/8/4k3/8/3K4/8/8 w - - 0 1");
    ASSERT_EQ(state.getGameState(), GameState::DRAW);
}

TEST(GameState, FiftyMoveRule) {
    State state = fenToState("r1bqkb1r/pppp1ppp/2n5/4P3/2B3n1/5N2/PPP2PPP/RNBQK2R w KQkq - 100 5");
    ASSERT_EQ(state.getGameState(), GameState::DRAW);
}

// --- GET MOVES --- //

TEST(getMoves, whiteCheckmated) {
    State state{whiteBackRanked(), Color::White, 0b1111, std::nullopt};

    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(0, moves.size());
}

TEST(getMoves, whiteSavedByBishop) {
    State state{whiteSavedByBishop(), Color::White, 0b1111, std::nullopt};
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(1, moves.size());
}

TEST(getMoves, whiteMiddleGame) {
    State state{randomMiddleGame(), Color::White, 0b1111, std::nullopt};
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(32, moves.size());
}

TEST(getMoves, blackMiddleGame) {
    State state{randomMiddleGame(), Color::Black, 0b1111, std::nullopt};
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(44, moves.size());
}

TEST(getMoves, whiteCenterKnight) {
    State state{centralWhiteKnight(), Color::White, 0b0000, std::nullopt};
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(16, moves.size());
}

TEST(getMoves, f3e5) {
    State state{};
    state.makeMove(Move::standardMove({6, 7}, {5, 5}));
    state.makeMove(Move::standardMove({4, 1}, {4, 3}));

    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(22, moves.size());
}

/// --- MAKE AND UNDO TESTS --- ///

TEST(MakeUndo, whitePawnTakesPawn) {
    State state{randomMiddleGame(), Color::White, 0b1111, std::nullopt};
    State stateCopy{state};

    state.makeMove(Move::standardMove({2, 4}, {1, 3}));
    ASSERT_EQ(0b1111, state.getCastlingRights());
    ASSERT_EQ(WHITE_PAWN, state.getBoard()(3, 1));
    ASSERT_EQ(EMPTY, state.getBoard()(4, 2));

    state.undoMove();
    ASSERT_EQ(stateCopy, state);
}

TEST(MakeUndo, whiteSavedByBishop) {
    State state{whiteSavedByBishop(), Color::White, 0b1111, std::nullopt};
    State stateCopy{state};
    std::vector<Move> moves = state.getMoves();
    state.makeMove(moves.at(0));
    ASSERT_EQ(0b1111, state.getCastlingRights());
    ASSERT_EQ(EMPTY, state.getBoard()(6, 4));
    ASSERT_EQ(WHITE_BISHOP, state.getBoard()(7, 3));
    state.undoMove();
    ASSERT_EQ(stateCopy, state);
}

TEST(MakeUndo, whiteKnightCapture) {
    State state{smotheredMatePosition(), Color::White, 0b1111, std::nullopt};
    State stateCopy{state};

    ASSERT_EQ(BLACK_ROOK, state.getBoard()(3, 2));
    ASSERT_EQ(WHITE_KNIGHT, state.getBoard()(4, 4));

    state.makeMove(Move::standardMove({4, 4}, {2, 3}));
    ASSERT_EQ(0b1111, state.getCastlingRights());
    ASSERT_EQ(WHITE_KNIGHT, state.getBoard()(3, 2));
    ASSERT_EQ(EMPTY, state.getBoard()(4, 4));

    state.undoMove();
    ASSERT_EQ(stateCopy, state);
    ASSERT_EQ(BLACK_ROOK, state.getBoard()(3, 2));
    ASSERT_EQ(WHITE_KNIGHT, state.getBoard()(4, 4));
}

TEST(MakeUndo, whiteKingSideCastle) {
    State state{unBlockedCastle(), Color::White, 0b1111, std::nullopt};

    State stateCopy{state};
    state.makeMove(Move::castleMove({4, 7}, {6, 7}, CastleType::SHORT));
    ASSERT_EQ(0b0011, state.getCastlingRights());
    ASSERT_EQ(WHITE_KING, state.getBoard()(7, 6));
    ASSERT_EQ(EMPTY, state.getBoard()(7, 4));
    ASSERT_EQ(WHITE_ROOK, state.getBoard()(7, 5));
    ASSERT_EQ(EMPTY, state.getBoard()(7, 7));
    state.undoMove();

    ASSERT_EQ(stateCopy, state);
    ASSERT_EQ(EMPTY, state.getBoard()(7, 6));
    ASSERT_EQ(WHITE_KING, state.getBoard()(7, 4));
    ASSERT_EQ(EMPTY, state.getBoard()(7, 5));
    ASSERT_EQ(WHITE_ROOK, state.getBoard()(7, 7));
    ASSERT_EQ(0b1111, state.getCastlingRights());
}

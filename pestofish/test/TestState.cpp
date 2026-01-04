//
// Created by Kiril on 2025-09-06.
//
#include "ModuleOnly/Utils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "gtest/gtest.h"

#include "ModuleOnly/Move.hpp"
#include "State.hpp"
#include "Preft.hpp"
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
    State state = fenToState("8/P7/1kp5/8/8/8/PPP5/1K1r4 w - - 0 1");

    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(0, moves.size());
}

TEST(getMoves, whiteSavedByBishop) {
    State state = fenToState("8/P7/1kp5/8/8/8/PPP1B3/1K1r4 w - - 0 1");
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(1, moves.size());
}

TEST(getMoves, whiteMiddleGame) {
    State state = fenToState("rnb1kbnr/p1pp1ppp/4q3/1p4pP/2P5/3P4/PP2PPPP/RNBQKBNR w KQkq - 0 1");
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(30, moves.size());
}

TEST(getMoves, blackMiddleGame) {
    State state = fenToState("rnb1kbnr/p1pp1ppp/4q3/1p4pP/2P5/3P4/PP2PPPP/RNBQKBNR b KQkq - 0 1");
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(44, moves.size());
}

TEST(getMoves, whiteCenterKnight) {
    State state = fenToState("1k6/8/8/1K6/4N3/8/8/8 w - - 0 1");
    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(16, moves.size());
}

TEST(getMoves, f3e5) {
    State state{};
    std::optional<Nnue> nnue = std::nullopt;
    state.makeMove(Move::standardMove({6, 7}, {5, 5}), nnue);
    state.makeMove(Move::standardMove({4, 1}, {4, 3}), nnue);

    std::vector<Move> moves = state.getMoves();
    ASSERT_EQ(22, moves.size());
}

/// --- MAKE AND UNDO TESTS --- ///

TEST(MakeUndo, whitePawnTakesPawn) {
    State state = fenToState("rnb1kbnr/p1pp1ppp/4q3/1p4pP/2P5/3P4/PP2PPPP/RNBQKBNR w KQkq - 0 1");
    State stateCopy{state};
    std::optional<Nnue> nnue = std::nullopt;

    state.makeMove(Move::standardMove({2, 4}, {1, 3}), nnue);
    ASSERT_EQ(0b1111, state.getCastlingRights());
    ASSERT_EQ(WHITE_PAWN, state.getBoard().at(3, 1));
    ASSERT_EQ(EMPTY, state.getBoard().at(4, 2));

    state.undoMove(nnue);
    ASSERT_EQ(stateCopy, state);
}

TEST(MakeUndo, whiteSavedByBishop) {
    State state = fenToState("8/P7/1kp5/8/8/8/PPP1B3/1K1r4 w - - 0 1");
    State stateCopy{state};
    std::optional<Nnue> nnue = std::nullopt;

    std::vector<Move> moves = state.getMoves();
    state.makeMove(moves.at(0), nnue);
    ASSERT_EQ(0b0000, state.getCastlingRights());
    ASSERT_EQ(EMPTY, state.getBoard().at(6, 4));
    ASSERT_EQ(WHITE_BISHOP, state.getBoard().at(7, 3));
    state.undoMove(nnue);
    ASSERT_EQ(stateCopy, state);
}

TEST(MakeUndo, whiteKnightCapture) {
    State state = fenToState("r3k1nr/pppnrbpp/3r4/8/4N3/N5P1/PPPP1P1P/R1BQK2R w KQkq - 0 1");
    State stateCopy{state};
    std::optional<Nnue> nnue = std::nullopt;

    ASSERT_EQ(BLACK_ROOK, state.getBoard().at(2, 3));
    ASSERT_EQ(WHITE_KNIGHT, state.getBoard().at(4, 4));

    state.makeMove(Move::standardMove({4, 4}, {2, 3}), nnue);
    ASSERT_EQ(0b1111, state.getCastlingRights());
    ASSERT_EQ(WHITE_KNIGHT, state.getBoard().at(3, 2));
    ASSERT_EQ(EMPTY, state.getBoard().at(4, 4));

    state.undoMove(nnue);
    ASSERT_EQ(stateCopy, state);
    ASSERT_EQ(BLACK_ROOK, state.getBoard().at(2, 3));
    ASSERT_EQ(WHITE_KNIGHT, state.getBoard().at(4, 4));
}

TEST(MakeUndo, whiteKingSideCastle) {
    State state = fenToState("r3k2r/8/8/8/8/8/3PPP2/R3K2R w KQkq - 0 1");
    std::optional<Nnue> nnue = std::nullopt;

    State stateCopy{state};
    state.makeMove(Move::castleMove({4, 7}, {6, 7}, CastleType::SHORT), nnue);
    ASSERT_EQ(0b0011, state.getCastlingRights());
    ASSERT_EQ(WHITE_KING, state.getBoard().at(7, 6));
    ASSERT_EQ(EMPTY, state.getBoard().at(7, 4));
    ASSERT_EQ(WHITE_ROOK, state.getBoard().at(7, 5));
    ASSERT_EQ(EMPTY, state.getBoard().at(7, 7));
    state.undoMove(nnue);

    ASSERT_EQ(stateCopy, state);
    ASSERT_EQ(EMPTY, state.getBoard().at(7, 6));
    ASSERT_EQ(WHITE_KING, state.getBoard().at(7, 4));
    ASSERT_EQ(EMPTY, state.getBoard().at(7, 5));
    ASSERT_EQ(WHITE_ROOK, state.getBoard().at(7, 7));
    ASSERT_EQ(0b1111, state.getCastlingRights());
}

//
// Created by Kiril on 2025-08-31.
//

#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include "State.hpp"
#include <gtest/gtest.h>

#include "testBoard.hpp"
#include "ModuleOnly/parse.hpp"

using namespace Pieces;

typedef unsigned long long u64;

u64 Preft(State& state, int depth) {
    if (depth == 0)
        return 1ULL;

    u64 nodes = 0;

    std::vector<Move> moves = state.getMoves();
    for (const Move move : moves) {
        state.makeMove(move);
        nodes += Preft(state, depth - 1);
        state.undoMove();
    }

    return nodes;
}

void PreftDivide(State& state, int depth) {
    auto moves = state.getMoves();
    u64 total = 0;
    for (auto move : moves) {
        state.makeMove(move);
        u64 count = Preft(state, depth - 1);
        state.undoMove();
        std::cout << move << ": " << count << "\n";
        total += count;
    }
    std::cout << "Total: " << total << "\n";
}

u64 DebugPreft(State& state, int depth) {
    if (depth == 0)
        return 1ULL;

    u64 nodes = 0;
    std::vector<Move> moves = state.getMoves();

    for (const Move move : moves) {
        State before = state;

        state.makeMove(move);
        nodes += DebugPreft(state, depth - 1);
        state.undoMove();

        if (state != before) {
            std::cerr << "Undo failed for move: " << move << "\n";
            // std::cerr << "Before:\n" << before;
            // std::cerr << "After undo:\n" << state;
            throw std::runtime_error("Undo mismatch");
        }
    }

    return nodes;
}

// TEST(Preft, undoConsistency) {
//     State state;
//     EXPECT_NO_THROW(DebugPreft(state, 4));
// }
//
// TEST(Preft, undoInMiddleGame) {
//     State state{problemMiddleGame(), Color::Black, 0b0000, std::nullopt};
//     EXPECT_NO_THROW(DebugPreft(state, 4));
// }

TEST(Preft, depth1) {
    State state;
    ASSERT_EQ(Preft(state, 1), 20);
}
TEST(Preft, depth2) {
    State state;
    ASSERT_EQ(Preft(state, 2), 400);
}
TEST(Preft, depth3) {
    State state;
    ASSERT_EQ(Preft(state, 3),  8902);
}
TEST(Preft, depth4) {
    State state;
    ASSERT_EQ(Preft(state, 4), 197281);
}
// TEST(Preft, startpos5) {
//     State state;
//     ASSERT_EQ(Preft(state, 5), 4865609);
// }
TEST(Preft, Kiwipete1) {
    State state = fenToState("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    ASSERT_EQ(Preft(state,1), 48);
}
TEST(Preft, Kiwipete2) {
    State state = fenToState("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    ASSERT_EQ(Preft(state, 2), 2039);
}
TEST(Preft, Kiwipete3) {
    State state = fenToState("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    ASSERT_EQ(Preft(state, 3), 97862);
}
TEST(Preft, Kiwipete4) {
    State state = fenToState("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    ASSERT_EQ(Preft(state, 4), 4085603);
}
TEST(Preft, EndGame4) {
    State state = fenToState("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");
    ASSERT_EQ(Preft(state, 4), 43238);
}
TEST(Preft, MiddleGameWhite4) {
    State state = fenToState("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    ASSERT_EQ(Preft(state, 4), 422333);
}
TEST(Preft, MiddleGameBlack4) {
    State state = fenToState("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    ASSERT_EQ(Preft(state, 4), 422333);
}
TEST(Preft, TalkChessPos) {
    State state = fenToState("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    ASSERT_EQ(Preft(state, 3),  62379);
}
TEST(Preft, AltPreft4) {
    State state = fenToState("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    ASSERT_EQ(Preft(state, 4),  3894594);
}


// TEST(Preft, depth6) {
//     ASSERT_EQ(Perft(State{}, 6), 119060324);
// }

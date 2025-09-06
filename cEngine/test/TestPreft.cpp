//
// Created by Kiril on 2025-08-31.
//

#include <gtest/gtest.h>
import Types;
import State;

typedef unsigned long long u64;

u64 Perft(State state, int depth)
{
    if (depth == 0)
        return 1ULL;

    u64 nodes = 0;

    std::vector<Move> moves = state.getMoves();
    for (Move move : moves) {
        state.makeMove(move);
        nodes += Perft(state, depth - 1);
        state.undoMove();
    }

    return nodes;
}

void PerftDivide(State state, int depth) {
    auto moves = state.getMoves();
    u64 total = 0;
    for (auto move : moves) {
        state.makeMove(move);
        u64 count = Perft(state, depth - 1);
        state.undoMove();
        std::cout << move << ": " << count << "\n";
        total += count;
    }
    std::cout << "Total: " << total << "\n";
}

TEST(Preft, depth1) {
    PerftDivide(State{}, 3);
    ASSERT_EQ(Perft(State{}, 1), 20);
}
TEST(Preft, depth2) {
    ASSERT_EQ(Perft(State{}, 2), 400);
}
// TEST(Preft, depth3) {
//     ASSERT_EQ(Perft(State{}, 3),  8902);
// }
// TEST(Preft, depth4) {
//     ASSERT_EQ(Perft(State{}, 4), 197281);
// }
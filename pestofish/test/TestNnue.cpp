//
// Created by Kiril on 1/4/2026.
//
#include "Nnue.hpp"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "ModuleOnly/parse.hpp"

TEST(NNUE, startPos) {
    const State state = fenToState("r1bqk1nr/pppp1Qpp/2n5/2b1p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4");
    std::cout << "State created" << std::endl;
    Nnue nnue{};
    std::cout << "NNUE created" << std::endl;
    int x = nnue.setBoard(state.getBoard(), state.getActiveColor());
    std::cout << "Done " << x << std::endl;
}
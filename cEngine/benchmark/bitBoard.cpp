//
// Created by Kiril on 12/2/2025.
//

#include <string>
#include <unordered_map>

#include "benchmark/benchmark.h"

#include "State.hpp"
#include "ModuleOnly/parse.hpp"

static void inCheck(benchmark::State& state, const std::string &fen) {
    const State boardState = fenToState(fen);
    for (auto _ : state) {
        benchmark::DoNotOptimize(
            boardState.getBoard().getBitBoard().inCheck<Color::White>()
        );
        benchmark::DoNotOptimize(
            boardState.getBoard().getBitBoard().inCheck<Color::Black>()
        );
    }
}

void registerBitBoards(const std::unordered_map<std::string, std::string> &fens) {
    for (auto fen : fens) {
        std::string name = "In check " + fen.first;
        benchmark::RegisterBenchmark(
            name.c_str(),
            [fen](benchmark::State& st) { inCheck(st, fen.second); }
        );
    }
}

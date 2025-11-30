//
// Created by Kiril on 11/30/2025.
//

#include <string>
#include <unordered_map>

#include "benchmark/benchmark.h"

#include "State.hpp"
#include "ModuleOnly/parse.hpp"

static void getAll(benchmark::State& state, const std::string &fen) {
    State boardState = fenToState(fen);
    for (auto _ : state) {
        benchmark::DoNotOptimize(
            boardState.getBoard().getPseudoLegal(boardState.getActiveColor(), boardState.getEnPassantSquare(), boardState.getCastlingRights())
        );
    }
}

void registerMoveGeneration(const std::unordered_map<std::string, std::string> &fens) {
    for (auto fen : fens) {
        std::string name = "Get all for " + fen.first;
        benchmark::RegisterBenchmark(
            name.c_str(),
            [fen](benchmark::State& st) { getAll(st, fen.second); }
        );
    }
}

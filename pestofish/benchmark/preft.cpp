//
// Created by Kiril on 11/30/2025.
//

#include <benchmark/benchmark.h>
#include "Engine.hpp"
#include "Evaluator.hpp"
#include "ModuleOnly/Move.hpp"
#include "Preft.hpp"
#include "ModuleOnly/parse.hpp"
#include "State.hpp"

static void fenBench(benchmark::State& state, const std::string &fen, const int depth) {
    State boardState = fenToState(fen);

    for (auto _ : state) {
        benchmark::DoNotOptimize(Preft(boardState, depth));
    }
}

void registerPreft(const std::unordered_map<std::string, std::string> &fens) {
    static const std::vector<int> depths = {2, 4};

    for (auto depth : depths) {
        for (auto fen : fens) {
            std::string name = fen.first + " at depth " + std::to_string(depth);
            benchmark::RegisterBenchmark(
                name.c_str(),
                [fen, depth](benchmark::State& st) { fenBench(st, fen.second, depth); }
            );
        }
    }
}
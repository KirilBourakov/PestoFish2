//
// Created by Kiril on 11/30/2025.
//

#include <benchmark/benchmark.h>
#include "Engine.hpp"
#include "Evaluator.hpp"
#include "ModuleOnly/Move.hpp"
#include "testBoard.hpp"
#include "ModuleOnly/parse.hpp"
#include "State.hpp"

static void startPos(benchmark::State& state) {
    State boardState = fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    const auto depth = state.range(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(Preft(boardState, depth));
    }
}

BENCHMARK(startPos)->DenseRange(1, 5, 1);
BENCHMARK_MAIN();
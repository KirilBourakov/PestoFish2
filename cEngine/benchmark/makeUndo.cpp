//
// Created by Kiril on 11/30/2025.
//

#include "State.hpp"
#include "benchmark/benchmark.h"
#include "ModuleOnly/parse.hpp"


static void makeMove(benchmark::State& state, const std::string &fen) {
    State boardState = fenToState(fen);
    std::vector<Move> moves = boardState.getMoves();

    std::default_random_engine rng;
    std::uniform_int_distribution<std::size_t> distribution(0, moves.size() - 1);

    Move& move = moves[distribution(rng)];
    for (auto _ : state) {
        benchmark::DoNotOptimize([&boardState, &move]() {
            boardState.makeMove(move);
        });
    }
}

static void undoMove(benchmark::State& state, const std::string &fen) {
    State boardState = fenToState(fen);
    std::vector<Move> moves = boardState.getMoves();

    std::default_random_engine rng;
    std::uniform_int_distribution<std::size_t> distribution(0, moves.size() - 1);
    Move& move = moves[distribution(rng)];
    boardState.makeMove(move);

    for (auto _ : state) {
        benchmark::DoNotOptimize([&boardState, &move]() {
            boardState.undoMove();
        });
    }
}

void registerMakeUndo(const std::unordered_map<std::string, std::string> &fens) {
    for (auto fen : fens) {
        std::string name = "Make move for " + fen.first;
        benchmark::RegisterBenchmark(
            name.c_str(),
            [fen](benchmark::State& st) { makeMove(st, fen.second); }
        );
        name = "Undo move for " + fen.first;
        benchmark::RegisterBenchmark(
            name.c_str(),
            [fen](benchmark::State& st) { undoMove(st, fen.second); }
        );
    }
}
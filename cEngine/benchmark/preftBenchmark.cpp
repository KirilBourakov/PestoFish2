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


const std::unordered_map<std::string, std::string> fens {
    {"startpos", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"},
    {"mirrored beginning", "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "},
    {"talkchess pos", "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"},

    {"kiwipete", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"},
    {"black middlegame", "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 "},

    {"rook endgame", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"},
};

static const std::vector<int> depths = {2, 4};

static void fenBench(benchmark::State& state, const std::string &fen, const int depth) {
    State boardState = fenToState(fen);

    for (auto _ : state) {
        benchmark::DoNotOptimize(Preft(boardState, depth));
    }
}

void registerPreftBenchmark() {
    for (auto depth : depths) {
        for (auto fen : fens) {
            std::string name = fen.first + " at depth " + std::to_string(depth);
            benchmark::RegisterBenchmark(
                name.c_str(),
                [fen, depth](benchmark::State& st) { fenBench(st, fen.second, depth); }
            );
        }
    }

    for (size_t i = 0; i < fens.size(); ++i) {
        for (int depth = 1; depth <= 5; depth += 4) {

        }
    }
}
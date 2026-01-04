//
// Created by Kiril on 2025-08-27.
//
#pragma once
#include "ModuleOnly/Enums.hpp"
#include "State.hpp"
#include "Board/Board.hpp"

typedef unsigned long long u64;

inline u64 Preft(State& state, const int depth) {
    std::optional<Nnue> nnue = std::nullopt;
    if (depth == 0)
        return 1ULL;

    u64 nodes = 0;

    for (const Move move : state.getMoves()) {
        state.makeMove(move, nnue);
        nodes += Preft(state, depth - 1);
        state.undoMove(nnue);
    }

    return nodes;
}
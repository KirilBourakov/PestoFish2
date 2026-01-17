//
// Created by Kiril on 2025-08-27.
//
#pragma once
#include "pestofish/Core/Enums.hpp"
#include "pestofish/Core/State.hpp"
#include "pestofish/Core/Board/Board.hpp"

typedef unsigned long long u64;

inline u64 Preft(State& state, const int depth) {
    if (depth == 0)
        return 1ULL;

    u64 nodes = 0;

    for (const Move move : state.getMoves()) {
        state.makeMove(move, nullptr);
        nodes += Preft(state, depth - 1);
        state.undoMove(nullptr);
    }

    return nodes;
}
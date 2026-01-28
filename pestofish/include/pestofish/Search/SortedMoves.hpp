//
// Created by Kiril on 1/28/2026.
//

#pragma once
#include <vector>

#include "EngineOptions.hpp"
#include "pestofish/Core/Move.hpp"
#include "pestofish/Core/State.hpp"


class SortedMoves {
public:
    explicit SortedMoves(
        std::vector<Move>& inp, const OptionalMove& killer1, const OptionalMove& killer2, const State& currState,
        const OptionalMove& tt_move, HistoryTable& history, RngInfo& rng)
    : moves(inp)
    {
        scores.reserve(moves.size());
        for (Move& m : moves) {
            scores.push_back(get_move_score(m, killer1, killer2, currState, tt_move, history) + rng.random());
        }
    };


    Move& next();
    constexpr int size() const { return scores.size(); }

private:
    static int get_move_score(const Move& move, const OptionalMove& killer1, const OptionalMove& killer2, const State& currState,
                              const OptionalMove& tt_move, HistoryTable& history);

    std::vector<Move>& moves;
    std::vector<int> scores;
    int sorted = 0;
};

//
// Created by Kiril on 2025-09-11.
//
#pragma once
#include "ModuleOnly/Enums.hpp"
#include "State.hpp"
#include <unordered_map>

constexpr int INF = 32000;
constexpr int MATE_SCORE = 31000;
constexpr int MATE_THRESHOLD = 30000;

class Evaluator {
public:
    static bool isBetterEval(const Color color, const int currBest, const int value);

    static int evaluate(const State& state);

private:
    static int getSquareWiseEvalAndGamePhase(const State& state, bool& endgame);
};

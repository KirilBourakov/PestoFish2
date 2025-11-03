//
// Created by Kiril on 2025-09-11.
//

export module Evaluator;
import State;
import <unordered_map>;
import Enums;

export constexpr int INF = 32000;
export constexpr int MATE_SCORE = 31000;
export constexpr int MATE_THRESHOLD = 30000;

export class Evaluator{
public:
    static bool isBetterEval(const Color color, const int currBest, const int value);

    static int evaluate(const State &state);

private:
    static int getSquareWiseEvalAndGamePhase(const State &state, bool& endgame);
};

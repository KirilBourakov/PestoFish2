//
// Created by Kiril on 2025-09-11.
//

export module Evaluator;
import State;
import <unordered_map>;
import Enums;

export class Evaluator{
public:
    static double evalCurrState(State &state, int depth, double alpha, double beta);
    static bool isBetterEval(const Color color, const double currBest, const double value);

    static double evaluate(const State &state);

private:
    static int getSquareWiseEvalAndGamePhase(const State &state, bool& endgame);
};

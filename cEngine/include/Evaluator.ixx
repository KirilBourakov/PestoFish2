//
// Created by Kiril on 2025-09-11.
//

export module Evaluator;
import State;
import <unordered_map>;
import Types;

export class Evaluator{
public:
    static double evaluate(const State &state);

private:
    static int getSquareWiseEvalAndGamePhase(const State &state, bool& endgame);
};

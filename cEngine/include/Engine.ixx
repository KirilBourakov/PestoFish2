//
// Created by Kiril on 2025-08-27.
//

export module Engine;
import <optional>;
import <vector>;
import Board;
import Types;
import State;


export class Engine {
public:
    Engine();
    int add(int a, int b);

    Move getBestMove();

private:
    double evalCurrState(int depth, double alpha, double beta);
    bool isBetterEval(Color color, double currBest, double value);

    State state;
};

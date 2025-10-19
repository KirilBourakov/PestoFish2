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
    Move getBestMove();
    void makeEngineMove();

    State& getState(){return state;};
    void setState(const State& state){this->state = state;};

private:
    State state;
};

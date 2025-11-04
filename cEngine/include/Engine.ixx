//
// Created by Kiril on 2025-08-27.
//

export module Engine;
import <optional>;
import <vector>;
import Board;
import Enums;
import State;
import Move;
import Transposition;

export class Engine {
public:
    Move getBestMove();
    Move getBestMoveConcurrent();
    void makeEngineMove();

    State& getState(){return state;};
    void setState(const State& state){this->state = state;};

private:
    State state;
    Transposition::TranspositionTable transPosTable;

    int minimax(State &state, int depth, int alpha, int beta, const Move& playedMove);
};

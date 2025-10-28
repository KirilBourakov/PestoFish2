//
// Created by Kiril on 2025-08-23.
//
module;
#include <algorithm>
#include <execution>
#include <iostream>
#include <mutex>
#include <ostream>

module Engine;
import Move;
import State;
import Evaluator;


Engine::Engine() {
    state = State();
}

void Engine::makeEngineMove() {
    state.makeMove(getBestMove());
}

// TODO: implement ply so earlier mates are more prioritized
Move Engine::getBestMove() {
    int alpha = -INF;
    int beta = INF;
    Color rootColor = state.getActiveColor();

    std::optional<Move> bestMove = std::nullopt;
    int bestEval = (rootColor == Color::White) ? -INF : INF;

    std::vector<Move> possibleMoves = state.getMoves();
    for (Move move : possibleMoves) {
        state.makeMove(move);
        int eval = Evaluator::evalCurrState(state, 2, alpha, beta);
        state.undoMove();

        if (!bestMove.has_value() || Evaluator::isBetterEval(rootColor, bestEval, eval)) {
            bestMove = move;
            bestEval = eval;
        }

        if (rootColor == Color::White) {
            alpha = std::max(alpha, eval);
        }
        else {
            beta = std::min(beta, eval);
        }

        if (beta <= alpha) {
            break;
        }
    }
    return bestMove.value();
}

Move Engine::getBestMoveConcurrent() {
    int alpha = -INF;
    int beta = INF;
    Color rootColor = state.getActiveColor();

    std::optional<Move> bestMove = std::nullopt;
    int bestEval = (rootColor == Color::White) ? -INF : INF;

    std::vector<Move> possibleMoves = state.getMoves();

    std::mutex mtx;

    std::for_each(std::execution::par, possibleMoves.begin(), possibleMoves.end(),
    [&](const Move& move) {
        State localState = state.makeThreadCopy();
        localState.makeMove(move);
        const int eval = Evaluator::evalCurrState(localState, 4, -INF, INF);

        std::lock_guard<std::mutex> lock(mtx);
        if (!bestMove.has_value() || Evaluator::isBetterEval(rootColor, bestEval, eval)) {
            bestMove = move;
            bestEval = eval;
        }
    });

    return bestMove.value();
}
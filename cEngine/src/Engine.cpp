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

Move Engine::getBestMove() {
    double alpha = -std::numeric_limits<double>::infinity();
    double beta = std::numeric_limits<double>::infinity();
    Color rootColor = state.getActiveColor();

    std::optional<Move> bestMove = std::nullopt;
    double bestEval = (rootColor == Color::White)
                        ? -std::numeric_limits<double>::infinity()
                        :  std::numeric_limits<double>::infinity();

    std::vector<Move> possibleMoves = state.getMoves();
    for (Move move : possibleMoves) {
        state.makeMove(move);
        double eval = Evaluator::evalCurrState(state, 2, alpha, beta);
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
    double alpha = -std::numeric_limits<double>::infinity();
    double beta = std::numeric_limits<double>::infinity();
    Color rootColor = state.getActiveColor();

    std::optional<Move> bestMove = std::nullopt;
    double bestEval = (rootColor == Color::White)
                        ? -std::numeric_limits<double>::infinity()
                        :  std::numeric_limits<double>::infinity();

    std::vector<Move> possibleMoves = state.getMoves();

    std::mutex mtx;

    std::for_each(std::execution::par, possibleMoves.begin(), possibleMoves.end(),
    [&](const Move& move) {
        State localState = state.makeThreadCopy();
        localState.makeMove(move);
        double eval = Evaluator::evalCurrState(localState, 4, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());

        std::lock_guard<std::mutex> lock(mtx);
        if (!bestMove.has_value() || Evaluator::isBetterEval(rootColor, bestEval, eval)) {
            bestMove = move;
            bestEval = eval;
        }
    });

    return bestMove.value();
}
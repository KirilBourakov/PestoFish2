//
// Created by Kiril on 2025-08-23.
//
module;
#include <stdexcept>

module Engine;
import Move;
import State;
import Evaluator;

Engine::Engine() {
    state = State();
}

Move Engine::getBestMove() {
    double alpha = -std::numeric_limits<double>::infinity();
    double beta = std::numeric_limits<double>::infinity();
    Color rootColor = state.getActiveColor();

    std::optional<Move> bestMove = std::nullopt;
    double bestEval = (rootColor == WHITE)
                        ? -std::numeric_limits<double>::infinity()
                        :  std::numeric_limits<double>::infinity();

    std::vector<Move> possibleMoves = state.getMoves();
    for (Move move : possibleMoves) {
        state.makeMove(move);
        double eval = evalCurrState(3, alpha, beta);
        state.undoMove();
        if (!bestMove.has_value() || isBetterEval(rootColor, bestEval, eval)) {
            bestMove = move;
            bestEval = eval;
        }

        if (rootColor == WHITE) {
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

double Engine::evalCurrState(int depth, double alpha, double beta) {
    if (depth == 0) {
        return Evaluator::evaluate(state);
    }

    if (state.isHalfMoveTie()) {
        return 0;
    }
    // TODO: implement 3 move repetition

    const std::vector<Move> possibleMoves = state.getMoves();
    if (possibleMoves.empty()) {
        // is the current color in check, other color wins
        if (state.colorInCheck(state.getActiveColor())) {
            double returnVal = std::numeric_limits<double>::infinity();
            if (state.getActiveColor() == WHITE) {
                returnVal *= -1;
            }
            return returnVal;
        }
        // if not, tie
        return 0;
    }

    double bestEval = (state.getActiveColor() == WHITE)
                        ? -std::numeric_limits<double>::infinity()
                        :  std::numeric_limits<double>::infinity();
    for (Move move : possibleMoves) {
        state.makeMove(move);
        double eval = evalCurrState(depth-1, alpha, beta);
        state.undoMove();

        if (isBetterEval(state.getActiveColor(), bestEval, eval)) {
            bestEval = eval;
        }

        // alpha beta pruning
        if (state.getActiveColor() == WHITE) {
            alpha = std::max(alpha, eval);
        }
        else {
            beta = std::min(beta, eval);
        }
        if (beta <= alpha) {
            break;
        }
    }
    return bestEval;
}

bool Engine::isBetterEval(Color color, double currBest, double value) {
    if (color == WHITE) {
        return value > currBest;
    } else {
        return value < currBest;
    }
}
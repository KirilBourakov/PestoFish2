//
// Created by Kiril on 2025-08-23.
//
module;

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
    double bestEval = (rootColor == WHITE)
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
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
        int eval;
        Move moveOut;
        Transposition::CutoffType cutoff;

        if (const bool hit = transPosTable.lookup(state.getZobrist(), eval, moveOut, cutoff); !hit) {
            eval = minimax(state, 3, alpha, beta, move);
        }
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

int Engine::minimax(State &state, int depth, int alpha, int beta, const Move& playedMove) {
    // --- Probe Transpose Table ---
    const uint64_t zobrist = state.getZobrist();

    int ttEval;
    Move ttMove;
    Transposition::CutoffType cutoffType;

    if (transPosTable.lookup(zobrist, ttEval, ttMove, cutoffType)) {
        switch (cutoffType) {
            case Transposition::CutoffType::EXACT:
                return ttEval;
            case Transposition::CutoffType::LOWER_BOUND:
                alpha = std::max(alpha, ttEval);
                break;
            case Transposition::CutoffType::UPPER_BOUND:
                beta = std::min(beta, ttEval);
                break;
            default:
                throw std::invalid_argument("Invalid cutoff type");
        }
        if (alpha >= beta) return ttEval;
    }

    // --- Check we should stop ---
    if (depth == 0) {
        return Evaluator::evaluate(state);
    }

    const std::vector<Move> possibleMoves = state.getMoves();
    GameState currGameState = state.getGameState(possibleMoves);
    if (currGameState == GameState::DRAW || currGameState == GameState::STALEMATE) {
        return 0;
    }
    if (currGameState == GameState::BLACK_WIN) {
        return -MATE_SCORE;
    }
    if (currGameState == GameState::WHITE_WIN) {
        return MATE_SCORE;
    }

    // --- Run a layer of minimax ---
    int bestEval = (state.getActiveColor() == Color::White) ? -INF : INF;
    Move bestMove;

    bool cutoffOccurred = false;
    int alphaOrig = alpha;
    for (Move move : possibleMoves) {
        state.makeMove(move);
        int eval = minimax(state, depth-1, alpha, beta, move);
        state.undoMove();

        if (Evaluator::isBetterEval(state.getActiveColor(), bestEval, eval)) {
            bestEval = eval;
            bestMove = move;
        }

        // alpha beta pruning
        if (state.getActiveColor() == Color::White) {
            alpha = std::max(alpha, eval);
        }
        else {
            beta = std::min(beta, eval);
        }
        if (beta <= alpha) {
            cutoffOccurred = true;
            break;
        }
    }

    // minimizing has a way to get beta, and this is greater then that, so they cut
    if (cutoffOccurred) {
        cutoffType = Transposition::CutoffType::LOWER_BOUND;
    }
    // maximizing has a way to get alphaOrig, and this path is worse then that, so we cut
    else if (bestEval <= alphaOrig) {
        cutoffType = Transposition::CutoffType::UPPER_BOUND;
    }
    else {
        cutoffType = Transposition::CutoffType::EXACT;
    }

    // using full move clock for age
    transPosTable.insert(state.getZobrist(), bestMove, depth, bestEval, cutoffType, state.getFullMoveClock());
    return bestEval;
}

// TODO: rework with efficient concurrency
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
        const int eval = minimax(localState, 4, -INF, INF, move);

        std::lock_guard<std::mutex> lock(mtx);
        if (!bestMove.has_value() || Evaluator::isBetterEval(rootColor, bestEval, eval)) {
            bestMove = move;
            bestEval = eval;
        }
    });

    return bestMove.value();
}


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


    int _; Transposition::CutoffType __;
    Move move_out = Move::invalid();
    transPosTable.lookup(state.getZobrist(), _, move_out, __);

    std::vector<Move> possibleMoves = state.getMoves();
    std::ranges::sort(possibleMoves,[this, move_out](const Move &a, const Move &b) {
        return get_move_score(a, this->state, move_out, this->globalHistory) > get_move_score(b, this->state, move_out, this->globalHistory);
    });


    for (Move move : possibleMoves) {
        state.makeMove(move);
        int eval = minimax(state, 0, 3, alpha, beta, globalHistory);
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

int Engine::minimax(State &state, int curr_depth, int max_depth, int alpha, int beta, HistoryTable& history) {
    // --- Probe Transpose Table ---
    const uint64_t zobrist = state.getZobrist();

    int ttEval;
    Move move_out = Move::invalid();
    Transposition::CutoffType cutoffType;

    if (transPosTable.lookup(zobrist, ttEval, move_out, cutoffType)) {
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
    if (curr_depth == max_depth) {
        return Evaluator::evaluate(state);
    }

    std::vector<Move> possibleMoves = state.getMoves();
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

    std::ranges::sort(possibleMoves,[state, move_out, &history](const Move &a, const Move &b) {
        return get_move_score(a, state, move_out, history) > get_move_score(b, state, move_out, history);
    });

    // --- Run a layer of minimax ---
    int bestEval = (state.getActiveColor() == Color::White) ? -INF : INF;
    Move bestMove;

    bool cutoffOccurred = false;
    const int alphaOrig = alpha;
    for (Move move : possibleMoves) {
        state.makeMove(move);
        int eval = minimax(state, curr_depth+1, max_depth, alpha, beta, history);
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
        history[bestMove] = (max_depth - curr_depth) * (max_depth - curr_depth);
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
    transPosTable.insert(state.getZobrist(), bestMove, max_depth-curr_depth, bestEval, cutoffType, state.getFullMoveClock());
    return bestEval;
}

int Engine::get_move_score(const Move& move, const State& state, const std::optional<Move>& tt_move, HistoryTable& history) {
    if (tt_move.has_value() && move == tt_move.value()) {
        return 1000000;
    }
    if (state.getAt(move.end) != Pieces::EMPTY) {
        const int victim_value = orderingValue.at(Pieces::piece_type(state.getAt(move.end)));
        const int attacker_value = orderingValue.at(Pieces::piece_type(state.getAt(move.start)));
        return 500000 + victim_value * 10 - attacker_value;
    }
    if (move.enPassantCapture) {
        return 500000;
    }
    return history[move];
}
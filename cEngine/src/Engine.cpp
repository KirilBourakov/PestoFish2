//
// Created by Kiril on 2025-08-23.
//
#include "Engine.hpp"

#include <algorithm>
#include <execution>
#include <iostream>
#include <mutex>
#include <ostream>

#include "Evaluator.hpp"
#include "Move.hpp"

void Engine::makeEngineMove() {
    state.makeMove(getBestMove());
}

// TODO: implement ply so earlier mates are more prioritized
Move Engine::getBestMove() {
    Transposition::Entry entry_out;
    transPosTable.lookup(state.getZobrist(), entry_out);

    std::vector<Move> possibleMoves = state.getMoves();
    std::ranges::sort(possibleMoves, [this, &entry_out](const Move& a, const Move& b) {
        return get_move_score(a, this->state, entry_out.bestMove, this->globalHistory) >
               get_move_score(b, this->state, entry_out.bestMove, this->globalHistory);
    });

    // single threaded depth 1
    int score = 0;
    int alpha = -INF;
    int beta = INF;
    Move bestMove = root(state, possibleMoves, 1, alpha, beta, globalHistory, score);

    int expected = score;
    int window = 75;

    // std::array<std::thread, 4> helpers;
    // TODO: locate making move from wrong side bug
    for (int max_depth = 2; max_depth <= 3; max_depth++) {
        alpha = expected - window;
        beta = expected + window;

        while (true) {
            const int original_alpha = alpha;
            const int original_beta = beta;

            // for (int i = 0; i < helpers.size(); i++) {
            //     HistoryTable history = globalHistory;
            //     int real_max = max_depth + (i % 2 == 1 ? 1 : 0);
            //
            //     std::vector<Move>& movesToView = possibleMoves;
            //     if (i == 0 && movesToView.size() >= 2) {
            //         std::vector<Move> view = possibleMoves;
            //         std::swap(view[0], view[1]);
            //         movesToView = view;
            //     }
            //     int scoreOut;
            //     helpers[i] = std::thread([this, &movesToView, real_max, alpha, beta, &history, &scoreOut]() mutable {
            //         State state = this->state.makeThreadCopy();
            //         this->root(state, movesToView, real_max, alpha, beta, history, scoreOut);
            //     });
            // }

            bestMove = root(state, possibleMoves, max_depth, alpha, beta, globalHistory, score);

            // stop.store(true, std::memory_order_relaxed);
            // // when true root and minimax break out as soon as possible
            // for (auto & helper : helpers) {
            //     helper.join();
            // }
            // stop.store(false, std::memory_order_release);

            if (score <= original_alpha) { // fail low
                alpha = original_alpha - window * 2;
                beta = original_beta;
            } else if (score >= original_beta) { // fail high
                alpha = original_alpha;
                beta = original_beta + window * 2;
            } else {
                break; // inside window
            }
            window *= 2;
        }

        expected = score;
    }

    return bestMove;
}

Move Engine::root(State& currState, const std::vector<Move>& rootMoves, const int maxDepth, int& alpha, int& beta,
                  HistoryTable& history, int& scoreOut) {
    std::optional<Move> bestMove = std::nullopt;
    const Color rootColor = currState.getActiveColor();
    int bestEval = (rootColor == Color::White) ? -INF : INF;

    const int alphaOrig = alpha;
    bool cutoffOccurred = false;
    for (Move move : rootMoves) {
        currState.makeMove(move);
        int eval = minimax(currState, 0, maxDepth, alpha, beta, history);
        currState.undoMove();

        if (!bestMove.has_value() || Evaluator::isBetterEval(rootColor, bestEval, eval)) {
            bestMove = move;
            bestEval = eval;
        }

        if (rootColor == Color::White) {
            alpha = std::max(alpha, eval);
        } else {
            beta = std::min(beta, eval);
        }

        if (beta <= alpha) {
            cutoffOccurred = true;
            break;
        }
        if (stop.load(std::memory_order_relaxed)) {
            break;
        }
    }

    Transposition::CutoffType cutoffType;
    if (cutoffOccurred) {
        history[bestMove.value()] += maxDepth * maxDepth;
        cutoffType = Transposition::CutoffType::LOWER_BOUND;
    } else if (bestEval <= alphaOrig) {
        cutoffType = Transposition::CutoffType::UPPER_BOUND;
    } else {
        cutoffType = Transposition::CutoffType::EXACT;
    }

    transPosTable.insert(currState.getZobrist(), bestMove.value(), maxDepth, bestEval, cutoffType, currState.getFullMoveClock());

    scoreOut = bestEval;
    return bestMove.value();
}

int Engine::minimax(State& currState, int curr_depth, int max_depth, int alpha, int beta, HistoryTable& history) {
    // --- Probe Transpose Table ---
    const uint64_t zobrist = currState.getZobrist();

    Transposition::Entry entry_out;
    if (transPosTable.lookup(zobrist, entry_out)) {
        if (entry_out.depth >= (max_depth - curr_depth)) {
            switch (entry_out.cutoffType) {
            case Transposition::CutoffType::EXACT:
                return entry_out.score;
            case Transposition::CutoffType::LOWER_BOUND:
                alpha = std::max(alpha, static_cast<int>(entry_out.score));
                break;
            case Transposition::CutoffType::UPPER_BOUND:
                beta = std::min(beta, static_cast<int>(entry_out.score));
                break;
            default:
                throw std::invalid_argument("Invalid cutoff type");
            }
            if (alpha >= beta) {
                return entry_out.score;
            }
        }
    }

    std::vector<Move> possibleMoves = currState.getMoves();
    GameState currGameState = currState.getGameState(possibleMoves);
    if (currGameState == GameState::DRAW || currGameState == GameState::STALEMATE) {
        return 0;
    }
    if (currGameState == GameState::BLACK_WIN) {
        return -MATE_SCORE;
    }
    if (currGameState == GameState::WHITE_WIN) {
        return MATE_SCORE;
    }

    // --- Check we should stop ---
    if (curr_depth >= max_depth || stop.load(std::memory_order_relaxed)) {
        return Evaluator::evaluate(currState);
    }

    std::ranges::sort(possibleMoves, [currState, &entry_out, &history](const Move& a, const Move& b) {
        return get_move_score(a, currState, entry_out.bestMove, history) > get_move_score(b, currState, entry_out.bestMove, history);
    });

    // --- Run a layer of minimax ---
    int bestEval = (currState.getActiveColor() == Color::White) ? -INF : INF;
    Move bestMove;

    bool cutoffOccurred = false;
    const int alphaOrig = alpha;
    for (const Move move : possibleMoves) {
        currState.makeMove(move);
        int eval = minimax(currState, curr_depth + 1, max_depth, alpha, beta, history);
        currState.undoMove();

        if (Evaluator::isBetterEval(currState.getActiveColor(), bestEval, eval)) {
            bestEval = eval;
            bestMove = move;
        }

        // alpha beta pruning
        if (currState.getActiveColor() == Color::White) {
            alpha = std::max(alpha, eval);
        } else {
            beta = std::min(beta, eval);
        }
        if (beta <= alpha) {
            cutoffOccurred = true;
            break;
        }
        if (stop.load(std::memory_order_relaxed)) {
            break;
        }
    }

    // minimizing has a way to get beta, and this is greater then that, so they
    // cut
    Transposition::CutoffType cutoffType;
    if (cutoffOccurred) {
        history[bestMove] += (max_depth - curr_depth) * (max_depth - curr_depth);
        cutoffType = Transposition::CutoffType::LOWER_BOUND;
    }
    // maximizing has a way to get alphaOrig, and this path is worse then that,
    // so we cut
    else if (bestEval <= alphaOrig) {
        cutoffType = Transposition::CutoffType::UPPER_BOUND;
    } else {
        cutoffType = Transposition::CutoffType::EXACT;
    }

    // using full move clock for age
    transPosTable.insert(currState.getZobrist(), bestMove, max_depth - curr_depth, bestEval, cutoffType, currState.getFullMoveClock());
    return bestEval;
}

int Engine::get_move_score(const Move& move, const State& currState, const std::optional<Move>& tt_move, HistoryTable& history) {
    if (tt_move.has_value() && move == tt_move.value()) {
        return 1000000;
    }
    if (currState.getAt(move.end) != Pieces::EMPTY) {
        const int victim_value = orderingValue.at(Pieces::piece_type(currState.getAt(move.end)));
        const int attacker_value = orderingValue.at(Pieces::piece_type(currState.getAt(move.start)));
        return 500000 + victim_value * 10 - attacker_value;
    }
    if (move.enPassantCapture) {
        return 500000;
    }
    return history[move];
}

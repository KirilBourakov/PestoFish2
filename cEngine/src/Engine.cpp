//
// Created by Kiril on 2025-08-23.
//
#include "Engine.hpp"

#include <algorithm>
#include <execution>
#include <iostream>
#include <mutex>
#include <ostream>
#include <thread>

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

    int stateSeed = 1;
    std::mt19937 rng(stateSeed);
    std::uniform_int_distribution<int> dist(0, 10);

    std::ranges::sort(possibleMoves, [this, &entry_out, &rng, &dist](const Move& a, const Move& b) {
        return get_move_score(a, this->state, entry_out.bestMove, this->globalHistory, rng, dist) >
               get_move_score(b, this->state, entry_out.bestMove, this->globalHistory, rng, dist);
    });

    int scoreOut;
    Move out = root(state, possibleMoves, 1, -INF, INF, globalHistory, scoreOut, 1);

    int expected = scoreOut;
    int window = 40;
    for (int depth = 2; depth <= 3; depth++) {
        int alpha = expected - window;
        int beta = expected + window;
        while (true) {
            int newScore;
            Move candidate = root(state, possibleMoves, depth, alpha, beta, globalHistory, newScore, 0);

            // stop.store(true, std::memory_order_relaxed);
            //  when true root and minimax break out as soon as possible
            // stop.store(false, std::memory_order_release);

            if (newScore <= alpha) { // fail low
                alpha = -INF;
            } else if (newScore >= beta) { // fail high
                beta = INF;
            } else {
                out = candidate;
                expected = newScore;
                break;
            }
        }
    }

    std::cout << "Score: " << scoreOut << std::endl;

    return out;

    // Move bestMove = root(state, possibleMoves, 3, alpha, beta, globalHistory, score, 1);

    // int expected = score;
    // int window = 40;
    //
    // constexpr int NUM_THREADS = 6;
    // std::array<std::thread, NUM_THREADS> helpers;
    // // TODO: locate making move from wrong side bug
    // for (int max_depth = 2; max_depth <= 3; max_depth++) {
    //     alpha = expected - window;
    //     beta = expected + window;
    //
    //     while (true) {
    //         const int original_alpha = alpha;
    //         const int original_beta = beta;
    //
    //         // TODO: break sync of threads
    //         for (int i = 0; i < helpers.size(); i++) {
    //             HistoryTable history = globalHistory;
    //
    //             int scoreOut;
    //             State state_copy = this->state.makeThreadCopy();
    //
    //             int real_depth = (i % 2 == 0) ? max_depth + 1 : max_depth;
    //
    //             helpers[i] =
    //                 std::thread([this, state_copy, &possibleMoves, real_depth, alpha, beta, &history, &scoreOut, i]() mutable {
    //                     this->root(state_copy, possibleMoves, real_depth, alpha, beta, history, scoreOut, i + 1);
    //                 });
    //         }
    //         Move candidate = root(state, possibleMoves, max_depth, alpha, beta, globalHistory, score, 0);
    //
    //         stop.store(true, std::memory_order_relaxed);
    //         // when true root and minimax break out as soon as possible
    //         for (auto& helper : helpers) {
    //             helper.join();
    //         }
    //         stop.store(false, std::memory_order_release);
    //
    //         if (score <= original_alpha) { // fail low
    //             alpha = -INF;
    //         } else if (score >= original_beta) { // fail high
    //             beta = INF;
    //         } else {
    //             bestMove = candidate;
    //             break; // inside window
    //         }
    //     }
    //
    //     expected = score;
    // }

    // return bestMove;
}

Move Engine::root(State& currState, const std::vector<Move>& rootMoves, const int depth, int alpha, int beta, HistoryTable& history,
                  int& scoreOut, const int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 10);

    std::optional<Move> bestMove = std::nullopt;
    int bestEval = -INF;
    const int colorRep = currState.getActiveColor() == Color::White ? 1 : -1;
    for (Move move : rootMoves) {
        currState.makeMove(move);
        int eval = -negamax(currState, depth - 1, -beta, -alpha, -colorRep, history, rng, dist);
        currState.undoMove();
        if (eval > bestEval) {
            bestEval = eval;
            bestMove = move;
        }
        alpha = std::max(alpha, bestEval);
        if (stop.load(std::memory_order_relaxed)) {
            break;
        }
    }
    scoreOut = bestEval;
    return bestMove.value();
}

int Engine::negamax(State& currState, int depth, int alpha, int beta, int colorRep, HistoryTable& history, std::mt19937& rng,
                    std::uniform_int_distribution<int>& dist) {
    int alphaOrig = alpha;

    Transposition::Entry entry_out;
    if (transPosTable.lookup(currState.getZobrist(), entry_out)) {
        if (entry_out.depth >= depth) {
            if (entry_out.cutoffType == Transposition::CutoffType::EXACT) {
                return entry_out.score;
            }
            if (entry_out.cutoffType == Transposition::CutoffType::LOWER_BOUND && entry_out.score >= beta) {
                return entry_out.score;
            }
            if (entry_out.cutoffType == Transposition::CutoffType::UPPER_BOUND && entry_out.score <= alpha) {
                return entry_out.score;
            }
        }
    }

    std::vector<Move> possibleMoves = currState.getMoves();
    GameState currGameState = currState.getGameState(possibleMoves);
    if (currGameState == GameState::DRAW || currGameState == GameState::STALEMATE) {
        return 0;
    }
    if (currGameState == GameState::WHITE_WIN) {
        return colorRep * +MATE_SCORE;
    }
    if (currGameState == GameState::BLACK_WIN) {
        return colorRep * -MATE_SCORE;
    }

    // --- Check we should stop ---
    if (stop.load(std::memory_order_relaxed)) {
        return colorRep * Evaluator::evaluate(currState);
    }

    if (depth == 0) {
        return colorRep * quiescence(currState, 15, alpha, beta);
        // return colorRep * Evaluator::evaluate(currState);
    }

    std::ranges::sort(possibleMoves, [&currState, &entry_out, &history, &rng, &dist](const Move& a, const Move& b) {
        return get_move_score(a, currState, entry_out.bestMove, history, rng, dist) >
               get_move_score(b, currState, entry_out.bestMove, history, rng, dist);
    });

    int bestValue = -INF;
    Move bestMove;
    for (const Move& move : possibleMoves) {
        currState.makeMove(move);
        int currValue = -negamax(currState, depth - 1, -beta, -alpha, -colorRep, history, rng, dist);
        currState.undoMove();
        if (currValue > bestValue) {
            bestValue = currValue;
            bestMove = move;
        }
        alpha = std::max(alpha, bestValue);
        if (alpha >= beta) {
            break;
        }
    }

    Transposition::CutoffType cutoffType;
    if (bestValue <= alphaOrig) {
        cutoffType = Transposition::CutoffType::UPPER_BOUND;
    } else if (bestValue >= beta) {
        cutoffType = Transposition::CutoffType::LOWER_BOUND;
    } else {
        cutoffType = Transposition::CutoffType::EXACT;
    }

    // using full move clock for age
    transPosTable.insert(currState.getZobrist(), bestMove, depth, bestValue, cutoffType, currState.getFullMoveClock());
    return bestValue;
}

int Engine::quiescence(State& state, const int depth, int alpha, int beta) {
    int staticEval = Evaluator::evaluate(state); // - eval means position good for black, else good for white

    int bestValue = staticEval;
    if (depth == 0 || stop.load(std::memory_order_relaxed)) {
        return bestValue;
    }
    if (bestValue >= beta) {
        return beta;
    }
    if (bestValue > alpha) {
        alpha = bestValue;
    }

    std::vector<Move> possibleMoves = state.getMoves();
    for (auto& move : possibleMoves) {
        if (state.getAt(move.end) != Pieces::EMPTY || move.enPassantCapture) { // TODO: support checks
            state.makeMove(move);
            const int score = -quiescence(state, depth - 1, -beta, -alpha);
            state.undoMove();

            if (score >= beta) {
                return score;
            }
            if (score > bestValue) {
                bestValue = score;
            }
            if (score > alpha) {
                alpha = score;
            }
        }
    }
    return bestValue;
}

int Engine::minimax(State& currState, int curr_depth, int max_depth, int alpha, int beta, HistoryTable& history, std::mt19937& rng,
                    std::uniform_int_distribution<int>& dist) {
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
    if (stop.load(std::memory_order_relaxed)) {
        return Evaluator::evaluate(currState);
    }

    if (curr_depth >= max_depth) {
        // return quiescence(currState, 3, alpha, beta);
        return Evaluator::evaluate(currState);
    }

    std::ranges::sort(possibleMoves, [currState, &entry_out, &history, &rng, &dist](const Move& a, const Move& b) {
        return get_move_score(a, currState, entry_out.bestMove, history, rng, dist) >
               get_move_score(b, currState, entry_out.bestMove, history, rng, dist);
    });

    // --- Run a layer of minimax ---
    int bestEval = (currState.getActiveColor() == Color::White) ? -INF : INF;
    Move bestMove;

    bool cutoffOccurred = false;
    const int alphaOrig = alpha;
    for (const Move move : possibleMoves) {
        currState.makeMove(move);
        int eval = minimax(currState, curr_depth + 1, max_depth, alpha, beta, history, rng, dist);

        if (Evaluator::isBetterEval(currState.getActiveColor(), bestEval, eval)) {
            bestEval = eval;
            bestMove = move;
        }

        currState.undoMove();

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

int Engine::get_move_score(const Move& move, const State& currState, const std::optional<Move>& tt_move, HistoryTable& history,
                           std::mt19937& rng, std::uniform_int_distribution<int>& dist) {
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
    return history[move] + dist(rng);
}

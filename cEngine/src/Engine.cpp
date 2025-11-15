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
#include "ModuleOnly/Move.hpp"

void Engine::makeEngineMove() {
    state.makeMove(getBestMove());
}

// TODO: implement ply so earlier mates are more prioritized
Move Engine::getBestMove() {
    int timeLimit = 1000;
    auto start = steadyClock::now();
    auto deadline = start + std::chrono::milliseconds(timeLimit);
    timeOut.store(false, std::memory_order_seq_cst);

    Transposition::Entry entry_out;
    transPosTable.lookup(state.getZobrist(), entry_out);

    std::vector<Move> possibleMoves = state.getMoves();

    int stateSeed = 1;
    std::mt19937 rng(stateSeed);
    std::uniform_int_distribution<int> dist(0, 10);

    std::ranges::sort(possibleMoves, [this, &entry_out, &rng, &dist](const Move& a, const Move& b) {
        return get_move_score(a, std::nullopt, std::nullopt, this->state, entry_out.bestMove, this->globalHistory, rng, dist) >
               get_move_score(b, std::nullopt, std::nullopt, this->state, entry_out.bestMove, this->globalHistory, rng, dist);
    });

    int scoreOut;
    Move out = root(state, possibleMoves, 1, -INF, INF, globalHistory, scoreOut, 1, deadline);

    int expected = scoreOut;
    int window = 40;

    constexpr int NUM_THREADS = 0; // turn shared SMP back on when threads are properly used (create once, use allways)
    std::array<std::thread, NUM_THREADS> helpers;
    for (int depth = 2; depth <= 20 && steadyClock::now() < deadline; depth++) {
        int alpha = expected - window;
        int beta = expected + window;
        while (true) {
            if (steadyClock::now() >= deadline) {
                stop.store(true, std::memory_order_relaxed);
                break;
            }

            // for (int i = 0; i < helpers.size(); i++) {
            //     HistoryTable history = globalHistory;
            //
            //     int scoreOut;
            //     State state_copy = this->state.makeThreadCopy();
            //
            //     int real_depth = (i % 2 == 0) ? depth + 1 : depth;
            //
            //     helpers[i] = std::thread(
            //         [this, state_copy, &possibleMoves, real_depth, alpha, beta, &history, &scoreOut, i, &deadline]() mutable {
            //             this->root(state_copy, possibleMoves, real_depth, alpha, beta, history, scoreOut, i + 1, deadline);
            //         });
            // }

            int newScore;
            Move candidate = root(state, possibleMoves, depth, alpha, beta, globalHistory, newScore, 0, deadline);

            stop.store(true, std::memory_order_seq_cst);
            // for (auto& helper : helpers) {
            //     helper.join();
            // }
            stop.store(false, std::memory_order_seq_cst);

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
}

Move Engine::root(State& currState, const std::vector<Move>& rootMoves, const int depth, int alpha, const int beta,
                  HistoryTable& history, int& scoreOut, const int seed, const steadyClock::time_point& deadline) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 10);

    KillerMoves killerMoves(depth);
    Move bestMove;
    int bestEval = -INF;
    const int colorRep = currState.getActiveColor() == Color::White ? 1 : -1;
    for (Move move : rootMoves) {
        if (steadyClock::now() >= deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        currState.makeMove(move);
        int eval = -negamax(currState, depth - 1, -beta, -alpha, -colorRep, history, rng, dist, killerMoves, deadline);
        currState.undoMove();
        if (eval > bestEval) {
            bestEval = eval;
            bestMove = move;
        }
        alpha = std::max(alpha, bestEval);

        if (endSearch()) {
            break;
        }
    }
    scoreOut = bestEval;
    return bestMove;
}

int Engine::negamax(State& currState, int depth, int alpha, int beta, int colorRep, HistoryTable& history, std::mt19937& rng,
                    std::uniform_int_distribution<int>& dist, KillerMoves& killerMoves, const steadyClock::time_point& deadline) {
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
    if (endSearch()) {
        return colorRep * Evaluator::evaluate(currState);
    }

    if (depth == 0) {
        return quiescence(currState, colorRep, 15, alpha, beta, deadline);
        // return colorRep * Evaluator::evaluate(currState);
    }

    std::ranges::sort(
        possibleMoves, [&currState, &entry_out, &killerMoves, &depth, &history, &rng, &dist](const Move& a, const Move& b) {
            return get_move_score(a, killerMoves.getFirst(depth), killerMoves.getSecond(depth), currState, entry_out.bestMove, history,
                                  rng, dist) > get_move_score(b, killerMoves.getFirst(depth), killerMoves.getSecond(depth), currState,
                                                              entry_out.bestMove, history, rng, dist);
        });

    int bestValue = -INF;
    Move bestMove;
    for (int i = 0; i < possibleMoves.size(); ++i) {
        if (steadyClock::now() >= deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        Move& move = possibleMoves[i];
        currState.makeMove(move);
        int currValue;
        // LMR
        int newDepth = depth - 1;
        if (depth >= 3 && i > 2 && !bestMove.enPassantCapture && currState.getAt(move.end) != Pieces::EMPTY) {
            const int reduction = static_cast<int>(.99 + std::log(depth) * std::log(i) / 3.14); // TODO: consider changing formula
            newDepth = depth - reduction;
        }
        if (i == 0) {
            currValue = -negamax(currState, newDepth, -beta, -alpha, -colorRep, history, rng, dist, killerMoves, deadline);
        } else {
            // principle variation search
            currValue = -negamax(currState, newDepth, -alpha - 1, -alpha, -colorRep, history, rng, dist, killerMoves, deadline);
            if (currValue > alpha && currValue < beta) {
                currValue = -negamax(currState, depth - 1, -beta, -alpha, -colorRep, history, rng, dist, killerMoves, deadline);
            }
        }
        currState.undoMove();
        if (currValue > bestValue) {
            bestValue = currValue;
            bestMove = move;
        }
        alpha = std::max(alpha, bestValue);
        if (alpha >= beta) {
            if (bestMove.enPassantCapture || currState.getAt(move.end) != Pieces::EMPTY) {
                killerMoves.insert(depth, bestMove);
            }
            break;
        }

        if (endSearch()) {
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

int Engine::quiescence(State& state, const int colorRep, const int depth, int alpha, int beta,
                       const steadyClock::time_point& deadline) {
    // TODO: add color rep int
    int staticEval = colorRep * Evaluator::evaluate(state); // - eval means position good for black, else good for white

    int bestValue = staticEval;
    if (depth == 0 || endSearch()) {
        return bestValue;
    }
    if (bestValue >= beta) {
        return bestValue;
    }
    if (bestValue > alpha) {
        alpha = bestValue;
    }

    std::vector<Move> possibleMoves = state.getMoves(); // TODO: limit to captures, and checks
    for (auto& move : possibleMoves) {
        if (state.getAt(move.end) != Pieces::EMPTY || move.enPassantCapture) { // TODO: support checks
            if (steadyClock::now() >= deadline) {
                stop.store(true, std::memory_order_relaxed);
                break;
            }

            state.makeMove(move);
            const int score = -quiescence(state, -colorRep, depth - 1, -beta, -alpha, deadline);
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

            if (endSearch()) {
                break;
            }
        }
    }
    // TODO: consider inserting into transpose table here.
    return bestValue;
}

int Engine::get_move_score(const Move& move, const std::optional<Move>& killer1, const std::optional<Move>& killer2,
                           const State& currState, const std::optional<Move>& tt_move, HistoryTable& history, std::mt19937& rng,
                           std::uniform_int_distribution<int>& dist) {
    if (tt_move.has_value() && move == tt_move.value()) {
        return 1000000;
    }
    if ((killer1.has_value() && move == killer1.value()) || (killer2.has_value() && move == killer2.value())) {
        return 900000;
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

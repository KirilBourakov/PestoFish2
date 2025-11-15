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
    SearchLimits searchD1 = {0, 1, -INF, INF, 0, deadline};
    Move out = root(state, possibleMoves, searchD1, globalHistory, scoreOut, 1);

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
            SearchLimits search = {0, depth, alpha, beta, 0, deadline};
            Move candidate = root(state, possibleMoves, search, globalHistory, newScore, 0);

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

Move Engine::root(State& currState, const std::vector<Move>& rootMoves, SearchLimits search, HistoryTable& history, int& scoreOut,
                  int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 10);

    OrderingInfo orderingInfo = OrderingInfo::create(history);
    Move bestMove;
    int bestEval = -INF;
    search.color = currState.getActiveColor() == Color::White ? 1 : -1;
    for (Move move : rootMoves) {
        if (steadyClock::now() >= search.deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        currState.makeMove(move);
        int eval = -negamax(currState, search.nextLimit(), orderingInfo, rng, dist);
        currState.undoMove();
        if (eval > bestEval) {
            bestEval = eval;
            bestMove = move;
        }
        search.alpha = std::max(search.alpha, bestEval);

        if (endSearch()) {
            break;
        }
    }
    scoreOut = bestEval;
    return bestMove;
}

int Engine::negamax(State& currState, SearchLimits search, OrderingInfo& orderingInfo, std::mt19937& rng,
                    std::uniform_int_distribution<int>& dist) {
    int alphaOrig = search.alpha;

    Transposition::Entry entry_out;
    if (transPosTable.lookup(currState.getZobrist(), entry_out)) {
        if (entry_out.depth >= search.depth) {
            if (entry_out.cutoffType == Transposition::CutoffType::EXACT) {
                return entry_out.score;
            }
            if (entry_out.cutoffType == Transposition::CutoffType::LOWER_BOUND && entry_out.score >= search.beta) {
                return entry_out.score;
            }
            if (entry_out.cutoffType == Transposition::CutoffType::UPPER_BOUND && entry_out.score <= search.alpha) {
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
        return search.color * +MATE_SCORE;
    }
    if (currGameState == GameState::BLACK_WIN) {
        return search.color * -MATE_SCORE;
    }

    // --- Check we should stop ---
    if (endSearch()) {
        return search.color * Evaluator::evaluate(currState);
    }

    if (search.depth == 0) {
        search.depth = 15;
        return quiescence(currState, search);
        // return colorRep * Evaluator::evaluate(currState);
    }

    std::ranges::sort(possibleMoves, [&currState, &entry_out, &orderingInfo, &search, &rng, &dist](const Move& a, const Move& b) {
        return get_move_score(a, orderingInfo.killer.getFirst(search.ply), orderingInfo.killer.getSecond(search.ply), currState,
                              entry_out.bestMove, orderingInfo.history, rng, dist) >
               get_move_score(b, orderingInfo.killer.getFirst(search.ply), orderingInfo.killer.getSecond(search.ply), currState,
                              entry_out.bestMove, orderingInfo.history, rng, dist);
    });

    int bestValue = -INF;
    Move bestMove;
    for (int i = 0; i < possibleMoves.size(); ++i) {
        if (steadyClock::now() >= search.deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        Move& move = possibleMoves[i];
        currState.makeMove(move);
        int currValue;
        // LMR
        int newDepth = search.depth - 1;
        if (search.depth >= 3 && i > 2 && !bestMove.enPassantCapture && currState.getAt(move.end) != Pieces::EMPTY) {
            const int reduction =
                static_cast<int>(.99 + std::log(search.depth) * std::log(i) / 3.14); // TODO: consider changing formula
            newDepth = search.depth - reduction;
        }
        if (i == 0) {
            currValue = -negamax(currState, search.nextLimit(newDepth), orderingInfo, rng, dist);
        } else {
            // principle variation search
            currValue = -negamax(currState, search.nextPVS(newDepth), orderingInfo, rng, dist);
            if (currValue > search.alpha && currValue < search.beta) {
                currValue = -negamax(currState, search.nextLimit(), orderingInfo, rng, dist);
            }
        }
        currState.undoMove();
        if (currValue > bestValue) {
            bestValue = currValue;
            bestMove = move;
        }
        search.alpha = std::max(search.alpha, bestValue);
        if (search.alpha >= search.beta) {
            if (!move.enPassantCapture && currState.getAt(move.end) == Pieces::EMPTY) {
                orderingInfo.killer.insert(search.ply, move);
            }
            orderingInfo.history[move] = search.depth * search.depth;
            break;
        }

        if (endSearch()) {
            break;
        }
    }

    Transposition::CutoffType cutoffType;
    if (bestValue <= alphaOrig) {
        cutoffType = Transposition::CutoffType::UPPER_BOUND;
    } else if (bestValue >= search.beta) {
        cutoffType = Transposition::CutoffType::LOWER_BOUND;
    } else {
        cutoffType = Transposition::CutoffType::EXACT;
    }

    // using full move clock for age
    transPosTable.insert(currState.getZobrist(), bestMove, search.depth, bestValue, cutoffType, currState.getFullMoveClock());
    return bestValue;
}

int Engine::quiescence(State& state, SearchLimits search) {
    // TODO: add color rep int
    int staticEval = search.color * Evaluator::evaluate(state); // - eval means position good for black, else good for white

    int bestValue = staticEval;
    if (search.depth == 0 || endSearch()) {
        return bestValue;
    }
    if (bestValue >= search.beta) {
        return bestValue;
    }
    if (bestValue > search.alpha) {
        search.alpha = bestValue;
    }

    std::vector<Move> possibleMoves = state.getMoves(); // TODO: limit to captures, and checks
    for (auto& move : possibleMoves) {
        if (state.getAt(move.end) != Pieces::EMPTY || move.enPassantCapture) { // TODO: support checks
            if (steadyClock::now() >= search.deadline) {
                stop.store(true, std::memory_order_relaxed);
                break;
            }

            state.makeMove(move);
            const int score = -quiescence(state, search.nextLimit());
            state.undoMove();

            if (score >= search.beta) {
                return score;
            }
            if (score > bestValue) {
                bestValue = score;
            }
            if (score > search.alpha) {
                search.alpha = score;
            }

            if (endSearch()) {
                break;
            }
        }
    }
    // TODO: consider inserting into transpose table here.
    return bestValue;
}

int Engine::get_move_score(const Move& move, const OptionalMove& killer1, const OptionalMove& killer2, const State& currState,
                           const OptionalMove& tt_move, HistoryTable& history, std::mt19937& rng,
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

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

Move Engine::getBestMove() {
    return getBestMove(SearchRequest{.movetime = 1000});
}

// TODO: implement ply so earlier mates are more prioritized
Move Engine::getBestMove(SearchRequest request) {
    int timelimit = request.movetime;
    int maxDepth = request.depth;
    bool infinite = request.infinite;

    // we were not given timelimit, depth nor infinite.
    // Let's calculate one from provided times, or drop to default if nothing is given
    if (timelimit == -1 && maxDepth == -1 && !infinite) {
        Color color = state.getActiveColor();
        int time = -1;
        int inc = 0;
        int movestogo = request.movestogo != -1 ? request.movestogo : 30;
        if (color == Color::Black) {
            time = request.btime;
            inc = request.binc;
        } else {
            time = request.wtime;
            inc = request.winc;
        }
        if (time != -1) {
            timelimit = time / (movestogo + 5) + inc * .8;
        }
        // default to a timelimit of 1 second
        else {
            timelimit = 1000;
        }
    }
    timeOut.store(false, std::memory_order_seq_cst);
    auto deadline = steadyClock::now();
    if (timelimit != -1) {
        deadline += std::chrono::milliseconds(timelimit);
    } else {
        deadline = steadyClock::time_point::max(); // No time provided
    }

    std::vector<Move> possibleMoves = request.searchmoves.value_or(state.getMoves());

    // Order
    int stateSeed = 1;
    Transposition::Entry entry_out;
    transPosTable.lookup(state.getZobrist(), entry_out);
    RngInfo rootRng = RngInfo::fromSeed(stateSeed);
    std::ranges::sort(possibleMoves, [this, &entry_out, &rootRng](const Move& a, const Move& b) {
        return get_move_score(a, std::nullopt, std::nullopt, this->state, entry_out.bestMove, this->globalHistory, rootRng) >
               get_move_score(b, std::nullopt, std::nullopt, this->state, entry_out.bestMove, this->globalHistory, rootRng);
    });

    KillerMoves killer{};
    OrderingInfo orderingInfo = {globalHistory, killer};

    constexpr int NUM_THREADS = 0; // turn shared SMP back on when threads are properly used (create once, use allways)
    std::array<std::thread, NUM_THREADS> helpers;

    Move out;
    int expected, scoreOut;
    for (int depth = 1; infinite || ((depth <= maxDepth || maxDepth == -1) && (steadyClock::now() < deadline)); depth++) {
        if (depth == 1) {
            SearchLimits search = {0, depth, -INF, INF, 0, deadline};
            out = root(state, possibleMoves, search, orderingInfo, scoreOut, rootRng);
            expected = scoreOut;
        } else {
            constexpr int window = 40;
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
                Move candidate = root(state, possibleMoves, search, orderingInfo, newScore, rootRng);

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
    }

    return out;
}

Move Engine::root(State& currState, const std::vector<Move>& rootMoves, SearchLimits search, OrderingInfo& orderingInfo, int& scoreOut,
                  RngInfo& rng) {
    Move bestMove;
    int bestEval = -INF;
    search.color = currState.getActiveColor() == Color::White ? 1 : -1;
    for (Move move : rootMoves) {
        if (steadyClock::now() >= search.deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        currState.makeMove(move);
        int eval = -negamax(currState, search.nextLimit(), orderingInfo, rng);
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

int Engine::negamax(State& currState, SearchLimits search, OrderingInfo& orderingInfo, RngInfo& rng) {
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

    std::ranges::sort(possibleMoves, [&currState, &entry_out, &orderingInfo, &search, &rng](const Move& a, const Move& b) {
        return get_move_score(a, orderingInfo.killer.getFirst(search.ply), orderingInfo.killer.getSecond(search.ply), currState,
                              entry_out.bestMove, orderingInfo.history, rng) >
               get_move_score(b, orderingInfo.killer.getFirst(search.ply), orderingInfo.killer.getSecond(search.ply), currState,
                              entry_out.bestMove, orderingInfo.history, rng);
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
            currValue = -negamax(currState, search.nextLimit(newDepth), orderingInfo, rng);
        } else {
            // principle variation search
            currValue = -negamax(currState, search.nextPVS(newDepth), orderingInfo, rng);
            if (currValue > search.alpha && currValue < search.beta) {
                currValue = -negamax(currState, search.nextLimit(), orderingInfo, rng);
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
                           const OptionalMove& tt_move, HistoryTable& history, RngInfo& rng) {
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
    return history[move] + rng.random();
}

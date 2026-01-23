//
// Created by Kiril on 2025-08-23.
//
#include "pestofish/Search/Engine.hpp"

#include <algorithm>
#include <execution>

#include "pestofish/Eval/Evaluator.hpp"
#include "pestofish/Core/Move.hpp"

void Engine::makeEngineMove() {
    state.makeMove(getBestMove(), &mainNnue);
}

void Engine::handleRequest(
    const SearchRequest& request, int &timelimit, int &maxDepth, bool &infinite,
    std::chrono::time_point<steadyClock> &deadline, std::vector<Move> &possibleMoves
) {
    constexpr int MIN_TIME_LIMIT = 50;

    timelimit = request.movetime;
    maxDepth = request.depth;
    infinite = request.infinite;

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
            timelimit = std::max(time, MIN_TIME_LIMIT);
        }
        // default to a timelimit of 1 second
        else {
            timelimit = 1000;
        }
    }

    deadline = steadyClock::now();
    if (timelimit != -1) {
        deadline += std::chrono::milliseconds(timelimit);
    } else {
        deadline = steadyClock::time_point::max();
    }

    possibleMoves = request.searchmoves.value_or(state.getMoves());
}


Move Engine::getBestMove() {
    return getBestMove(SearchRequest{.depth = 3});
}

// TODO: implement ply so earlier mates are more prioritized
Move Engine::getBestMove(const SearchRequest& request) {
    clearBoolFlags();

    int timelimit;
    int maxDepth;
    bool infinite;
    std::chrono::time_point<steadyClock> deadline;
    std::vector<Move> possibleMoves;
    handleRequest(request, timelimit, maxDepth, infinite, deadline, possibleMoves);

    // Order
    int stateSeed = 1;
    Transposition::Entry entry_out;
    transPosTable.lookup(state.getZobrist(), entry_out);
    RngInfo rootRng = RngInfo::fromSeed(stateSeed);
    std::ranges::sort(possibleMoves, [this, &entry_out](const Move& a, const Move& b) {
        return get_move_score(a, std::nullopt, std::nullopt, this->state, entry_out.bestMove, this->orderInfo.history, 0) >
               get_move_score(b, std::nullopt, std::nullopt, this->state, entry_out.bestMove, this->orderInfo.history, 0);
    });


    Move out;
    int expected, scoreOut, depth;
    for (depth = 1; infinite || ((depth <= maxDepth || maxDepth == -1) && (steadyClock::now() < deadline)); depth++) {
        //std::cout << static_cast<int>(steadyClock::now() < deadline) << std::endl;
        if (depth == 1) {
            SearchLimits search = {0, depth, -INF, INF, 0, deadline};
            out = root(state, possibleMoves, search, orderInfo, rootRng, scoreOut, mainNnue);
            expected = scoreOut;
            lazySmpThreads.sync(state, possibleMoves, mainNnue);
        } else {
            constexpr int window = 40;
            int alpha = expected - window;
            int beta = expected + window;
            while (true) {
                if (steadyClock::now() >= deadline) {
                    stop.store(true, std::memory_order_relaxed);
                    break;
                }

                SearchLimits search = {0, depth, alpha, beta, 0, deadline};
                lazySmpThreads.enqueue(
                    [this](State& currState, const std::vector<Move>& mvs, const SearchLimits &searchRef, OrderingInfo& orderingInfo,
                           RngInfo& rng, int& scoreOut, Nnue& nnue) {
                        return this->root(currState, mvs, searchRef, orderingInfo, rng, scoreOut, nnue); // TODO: give diff alpha/beta?
                    },
                    search);

                int newScore;
                Move candidate = root(state, possibleMoves, search, orderInfo, rootRng, newScore, mainNnue);

                stop.store(true, std::memory_order_seq_cst);
                lazySmpThreads.clearQueue();
                lazySmpThreads.waitForIdle();
                stop.store(false, std::memory_order_seq_cst);

                if (newScore <= alpha) { // fail low
                    alpha = -INF;
                } else if (newScore >= beta) { // fail high
                    if (!timeOut.load(std::memory_order_seq_cst) && !forceStop.load(std::memory_order_seq_cst)) {
                        out = candidate;
                    }
                    beta = INF;
                } else {
                    if (!timeOut.load(std::memory_order_seq_cst) && !forceStop.load(std::memory_order_seq_cst)) {
                        out = candidate;
                        expected = newScore;
                    }
                    break;
                }
            }
        }
       // std::cout << "Best Move " << out << std::endl;
    }
    // std::cout << "Depth searched " << depth << "Score: " << scoreOut << std::endl;
    //std::cout << "-----------" << std::endl;

    return out;
}

Move Engine::root(State& currState, const std::vector<Move>& rootMoves, SearchLimits search, OrderingInfo& orderingInfo, RngInfo& rng,
                  int& scoreOut, Nnue& nnue) {
    Move bestMove;
    int bestEval = -INF;
    search.color = currState.getActiveColor() == Color::White ? 1 : -1;
    for (Move move : rootMoves) {
        if (steadyClock::now() >= search.deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        currState.makeMove(move, &nnue);
        int eval = -negamax(currState, search.nextLimit(), orderingInfo, rng, nnue);
        currState.undoMove(&nnue);
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

int Engine::negamax(State& currState, SearchLimits search, OrderingInfo& orderingInfo, RngInfo& rng, Nnue& nnue) {
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
        return search.color * (+MATE_SCORE-search.ply);
    }
    if (currGameState == GameState::BLACK_WIN) {
        return search.color * (-MATE_SCORE+search.ply);
    }

    // --- Check we should stop ---
    if (endSearch()) {
        return search.color * nnue.eval(currState.getActiveColor());
    }

    if (search.depth == 0) {
        search.depth = 15;
        return quiescence(currState, search, nnue);
    }


    std::vector<std::pair<Move,int>> scored;
    scored.reserve(possibleMoves.size());
    for (auto& m : possibleMoves) {
        const int dsync = rng.random();
        scored.emplace_back(m, get_move_score(
            m, orderingInfo.killer.getFirst(search.ply), orderingInfo.killer.getSecond(search.ply),
            currState, entry_out.bestMove, orderingInfo.history, dsync
        ));
    }
    std::ranges::sort(scored, [](auto& a, auto& b){ return a.second > b.second; });

    int bestValue = -INF;
    std::optional<Move> bestMove = std::nullopt;
    for (int i = 0; i < scored.size(); ++i) {
        if (steadyClock::now() >= search.deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        Move& move = scored[i].first;
        currState.makeMove(move, &nnue);
        int currValue;

        // LMR
        int newDepth = search.depth - 1;

        bool isQuiet = (currState.getAt(move.end) == Pieces::EMPTY) && !move.enPassantCapture;
        bool isPromotion = move.promotedTo.has_value(); // TODO: add check as exception also
        if (search.depth >= 3 && i > 2 && isQuiet && !isPromotion) {
            newDepth = search.depth - lmrTable(search.depth, i);
        }

        if (i == 0) {
            currValue = -negamax(currState, search.nextLimit(newDepth), orderingInfo, rng, nnue);
        } else {
            // principle variation search
            currValue = -negamax(currState, search.nextPVS(newDepth), orderingInfo, rng, nnue);
            if (currValue > search.alpha && currValue < search.beta) {
                currValue = -negamax(currState, search.nextLimit(), orderingInfo, rng, nnue);
            }
        }
        currState.undoMove(&nnue);
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

    if (bestMove.has_value() && !endSearch()) {
        transPosTable.insert(currState.getZobrist(), bestMove.value(), search.depth, bestValue, cutoffType, currState.getFullMoveClock());
    }
    return bestValue;
}

int Engine::quiescence(State& currState, SearchLimits search, Nnue& nnue) {
    // TODO: add color rep int
    int staticEval = search.color * nnue.eval(currState.getActiveColor()); // - eval means position good for black, else good for white

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

    if (Transposition::Entry entry_out; transPosTable.lookup(currState.getZobrist(), entry_out)) {
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

    std::vector<Move> possibleMoves;
    if (currState.activeColorInCheck()) {
        possibleMoves = currState.getMoves();
    } else {
        possibleMoves =  currState.getQuiescenceMoves();
    }

    int alphaOrig = search.alpha;
    std::optional<Move> bestMove = std::nullopt;
    for (auto& move : possibleMoves) {
        if (steadyClock::now() >= search.deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        currState.makeMove(move, &nnue);
        const int score = -quiescence(currState, search.nextLimit(), nnue);
        currState.undoMove(&nnue);

        if (score >= search.beta) {
            return score;
        }
        if (score > bestValue) {
            bestValue = score;
            bestMove = move;
        }
        if (score > search.alpha) {
            search.alpha = score;
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

    if (bestMove.has_value() && !endSearch()) {
        transPosTable.insert(
            currState.getZobrist(),
            bestMove.value(),
            Transposition::quiescence_depth,
            bestValue,
            cutoffType,
            currState.getFullMoveClock()
        );
    }

    return bestValue;
}

int Engine::get_move_score(const Move& move, const OptionalMove& killer1, const OptionalMove& killer2, const State& currState,
                           const OptionalMove& tt_move, HistoryTable& history, int dsync) {
    if (tt_move.has_value() && move == tt_move.value()) {
        return 1000000 + dsync;
    }
    if ((killer1.has_value() && move == killer1.value()) || (killer2.has_value() && move == killer2.value())) {
        return 900000 + dsync;
    }

    // Cheap way to move promotions up above non promotions
    if (move.promotedTo.has_value()) {
        const int promoteValue = orderingValue.at(Pieces::piece_type(move.promotedTo.value()));
        dsync += promoteValue;
    }

    if (currState.getAt(move.end) != Pieces::EMPTY) {
        const int victim_value = orderingValue.at(Pieces::piece_type(currState.getAt(move.end)));
        const int attacker_value = orderingValue.at(Pieces::piece_type(currState.getAt(move.start)));
        return 500000 + victim_value * 10 - attacker_value + dsync;
    }
    if (move.enPassantCapture) {
        return 500000 + dsync;
    }
    return history[move] + dsync;
}

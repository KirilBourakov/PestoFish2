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
    RngInfo rootRng = RngInfo::fromSeed(stateSeed);

    // depth 1
    const int color = state.getActiveColor() == Color::White ? 1 : -1;
    constexpr int depth = 1, ply = 0;
    const SearchLimits search = {color, depth, -INF, INF, ply, deadline};
    auto [out, expected] = searchMoves(state, possibleMoves, search, orderInfo, rootRng, mainNnue);

    // launch deepening
    lazySmpThreads.sync(state, possibleMoves, mainNnue);
    lazySmpThreads.enqueue(
    [this, infinite, maxDepth, deadline, expected](State& currState, const std::vector<Move>& mvs, OrderingInfo& orderingInfo,
           RngInfo& rng, Nnue& nnue) {
        return this->iterativeDeepening(currState, mvs, orderingInfo, rng, nnue, infinite, maxDepth, deadline, expected); // TODO: give diff alpha/beta?
    });

    const Move candidate = iterativeDeepening(
        state, possibleMoves, orderInfo, rootRng, mainNnue, infinite, maxDepth, deadline, expected
    );
    // Make sure we've done a full search at least once before overwriting
    if (candidate.isValid()) {
        out = candidate;
    }

    // clean up threads
    stop.store(true, std::memory_order_seq_cst);
    lazySmpThreads.clearQueue();
    lazySmpThreads.waitForIdle();
    stop.store(false, std::memory_order_seq_cst);

    return out;
}

// TODO: add different stuff to threads
Move Engine::iterativeDeepening(
    State& currState,
    const std::vector<Move>& moves,
    OrderingInfo& orderingInfo,
    RngInfo& rng,
    Nnue& nnue,
    bool infinite, int maxDepth, std::chrono::time_point<steadyClock> deadline,
    int expectedCenter
) {
    constexpr int window = 40;

    Move out;
    for (int depth = 2; infinite || ((depth <= maxDepth || maxDepth == -1) && (steadyClock::now() < deadline)); depth++) {
        int alpha = expectedCenter - window;
        int beta = expectedCenter + window;
        while (true) {
            if (steadyClock::now() >= deadline) {
                stop.store(true, std::memory_order_relaxed);
                break;
            }

            int color = currState.getActiveColor() == Color::White ? 1 : -1;
            SearchLimits search = {color, depth, alpha, beta, 0, deadline};
            auto candidate = searchMoves(currState, moves, search, orderingInfo, rng, nnue);

            if (candidate.second <= alpha) { // fail low
                alpha = -INF;
            } else if (candidate.second >= beta) { // fail high
                if (!timeOut.load(std::memory_order_seq_cst) && !forceStop.load(std::memory_order_seq_cst)) {
                    out = candidate.first;
                }
                beta = INF;
            } else {
                if (!timeOut.load(std::memory_order_seq_cst) && !forceStop.load(std::memory_order_seq_cst)) {
                    out = candidate.first;
                    expectedCenter = candidate.second;
                }
                break;
            }
        }
    }
    return out;
}

std::pair<Move, int> Engine::searchMoves(
    State& currState,
    const std::vector<Move>& rootMoves,
    SearchLimits search,
    OrderingInfo& orderingInfo,
    RngInfo& rng,
    Nnue& nnue
) {
    int alphaOrig = search.alpha;
    uint64_t entry_out = 0;
    const bool found = transPosTable.lookup(currState.getZobrist(), entry_out);
    if (found  && search.ply > 0) {
        if (Transposition::ttEntryDepth(entry_out) >= search.depth) {
            Transposition::CutoffType type = Transposition::ttEntryCutType(entry_out);
            int16_t score = Transposition::ttEntryScore(entry_out);
            Move mv{Transposition::ttEntryMove(entry_out)};
            if (type == Transposition::CutoffType::EXACT) {
                return {mv, score};
            }
            if (type == Transposition::CutoffType::LOWER_BOUND && score >= search.beta) {
                return {mv, score};
            }
            if (type == Transposition::CutoffType::UPPER_BOUND && score <= search.alpha) {
                return {mv, score};
            }
        }
    }

    std::vector<Move> possibleMoves;
    if (search.ply == 0) {
        possibleMoves = rootMoves;
    } else {
        possibleMoves = currState.getMoves();
    }


    GameState currGameState = currState.getGameState(possibleMoves);
    if (currGameState == GameState::DRAW || currGameState == GameState::STALEMATE) {
        return {{}, 0};
    }
    if (currGameState == GameState::WHITE_WIN) {
        return {{}, search.color * (+MATE_SCORE-search.ply)};
    }
    if (currGameState == GameState::BLACK_WIN) {
        return {{}, search.color * (-MATE_SCORE+search.ply)};
    }

    // --- Check we should stop ---
    if (endSearch()) {
        return {{}, search.color * nnue.eval(currState.getActiveColor())};
    }

    if (search.depth == 0) {
        search.depth = 15;
        return {{}, quiescence(currState, search, nnue)};
    }

    OptionalMove ttMove = found ? std::make_optional(Transposition::ttEntryMove(entry_out)) : std::nullopt;
    std::vector<std::pair<Move,int>> scored;
    scored.reserve(possibleMoves.size());
    for (auto& m : possibleMoves) {
        const int dsync = rng.random();
        scored.emplace_back(m, get_move_score(
            m, orderingInfo.killer.getFirst(search.ply), orderingInfo.killer.getSecond(search.ply),
            currState, ttMove, orderingInfo.history, dsync
        ));
    }
    std::ranges::sort(scored, [](auto& a, auto& b){ return a.second > b.second; });


    std::pair<Move,int> bestResult =  {{}, -INF};
    for (int i = 0; i < scored.size(); ++i) {
        if (steadyClock::now() >= search.deadline) {
            stop.store(true, std::memory_order_relaxed);
            break;
        }

        Move& move = scored[i].first;
        currState.makeMove(move, &nnue);

        // LMR
        const int newDepth = calculateLMRDepth(currState, move, i, search.depth);

        std::pair<Move,int> currResult;
        if (i == 0) {
            currResult = searchMoves(currState, rootMoves, search.nextLimit(newDepth), orderingInfo, rng, nnue);
            currResult.second = -currResult.second;
        } else {
            // principle variation search
            currResult = searchMoves(currState, rootMoves, search.nextPVS(newDepth), orderingInfo, rng, nnue);
            currResult.second = -currResult.second;
            if (currResult.second > search.alpha && currResult.second < search.beta) {
                currResult = searchMoves(currState, rootMoves, search.nextLimit(), orderingInfo, rng, nnue);
                currResult.second = -currResult.second;
            }
        }
        currState.undoMove(&nnue);
        if (currResult.second > bestResult.second) {
            bestResult.second = currResult.second;
            bestResult.first = move;
        }
        search.alpha = std::max(search.alpha, bestResult.second);
        if (search.alpha >= search.beta) {
            if (!move.getEnPassantCapture() && currState.getAt(move.getEnd()) == Pieces::EMPTY) {
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
    if (bestResult.second <= alphaOrig) {
        cutoffType = Transposition::CutoffType::UPPER_BOUND;
    } else if (bestResult.second >= search.beta) {
        cutoffType = Transposition::CutoffType::LOWER_BOUND;
    } else {
        cutoffType = Transposition::CutoffType::EXACT;
    }

    if (bestResult.first.isValid() && !endSearch()) {
        transPosTable.insert(currState.getZobrist(), bestResult.first, search.depth, bestResult.second, cutoffType, currState.getFullMoveClock());
    }
    return bestResult;
}

int Engine::calculateLMRDepth(const State& currState, const Move& move, const int i, const int depth) const {
    int newDepth = depth - 1;
    const bool isQuiet = (currState.getAt(move.getEnd()) == Pieces::EMPTY) && !move.getEnPassantCapture();
    const bool isPromotion = move.getPromotedTo().has_value(); // TODO: add check as exception also
    if (depth >= 3 && i > 2 && isQuiet && !isPromotion) {
        newDepth = depth - lmrTable(depth, i);
    }
    return newDepth;
}

int Engine::quiescence(State& currState, SearchLimits search, Nnue& nnue) {
    int staticEval = search.color * nnue.eval(currState.getActiveColor()); // - eval means position good for black, else good for white

    int bestValue = staticEval;
    if (search.depth == 0 || endSearch()) {
        return bestValue;
    }
    if (bestValue >= search.beta) {
        return search.beta;
    }
    if (bestValue > search.alpha) {
        search.alpha = bestValue;
    }

    if (uint64_t entry_out; transPosTable.lookup(currState.getZobrist(), entry_out)) {
        Transposition::CutoffType type = Transposition::ttEntryCutType(entry_out);
        int16_t score = Transposition::ttEntryScore(entry_out);
        if (type == Transposition::CutoffType::EXACT) {
            return score;
        }
        if (type == Transposition::CutoffType::LOWER_BOUND && score >= search.beta) {
            return score;
        }
        if (type == Transposition::CutoffType::UPPER_BOUND && score <= search.alpha) {
            return score;
        }
    }

    std::vector<Move> possibleMoves;
    if (currState.activeColorInCheck()) {
        possibleMoves = currState.getMoves();
        if (possibleMoves.empty()) {
            return -MATE_SCORE + search.ply;
        }
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
    if (move.getPromotedTo().has_value()) {
        const int promoteValue = orderingValue.at(Pieces::piece_type(move.getPromotedTo().value()));
        dsync += promoteValue;
    }

    if (currState.getAt(move.getEnd()) != Pieces::EMPTY) {
        const int victim_value = orderingValue.at(Pieces::piece_type(currState.getAt(move.getEnd())));
        const int attacker_value = orderingValue.at(Pieces::piece_type(currState.getAt(move.getStart())));
        return 500000 + victim_value * 10 - attacker_value + dsync;
    }
    if (move.getEnPassantCapture()) {
        return 500000 + dsync;
    }
    return history[move] + dsync;
}

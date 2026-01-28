//
// Created by Kiril on 2025-08-27.
#pragma once
#include <atomic>
#include <unordered_map>

#include "pestofish/Search/EngineOptions.hpp"
#include "pestofish/Core/Enums.hpp"
#include "pestofish/Core/Move.hpp"
#include "pestofish/Search/TranspositionTable.hpp"
#include "pestofish/Core/State.hpp"
#include <optional>
#include <vector>

#include "pestofish/Search/LazySmpThreads.hpp"

class Engine {
public:
    Engine()
        : lazySmpThreads(4, 13214, orderInfo) {}
    Engine(const int threads, const int seed)
        : lazySmpThreads(threads, seed, orderInfo) {}

    void makeEngineMove();

    Move getBestMove();
    Move getBestMove(const SearchRequest& request);

    State& getState() {
        return state;
    };
    void setState(const State& state) {
        this->state = state;
    };
    void updatedMainNnue() {
        mainNnue.setBoard(state.getBoard(), state.getActiveColor());
    }
    void stopSearch() {
        forceStop.store(true, std::memory_order_seq_cst);
    }

private:
    State state{};
    Transposition::TranspositionTable transPosTable{};
    OrderingInfo orderInfo{};
    const LMRLookUp lmrTable{};

    LazySmpThreads lazySmpThreads;
    std::atomic<bool> stop = false; // stop used by main thread to tell smp to stop
    std::atomic<bool> timeOut = false; // we ran out of time
    std::atomic<bool> forceStop = false; // UCI told us to stop

    Nnue mainNnue = {};

    Move iterativeDeepening(
        State& currState, const std::vector<Move>& moves, OrderingInfo& orderingInfo,
        RngInfo& rng, Nnue& nnue, bool infinite, int maxDepth, std::chrono::time_point<steadyClock> deadline, int expectedCenter
    );
    std::pair<Move, int> searchMoves(
        State& currState, const std::vector<Move>& rootMoves, SearchLimits search, OrderingInfo& orderingInfo, RngInfo& rng, Nnue& nnue
    );
    int calculateLMRDepth(const State& currState, const Move& move, int i, int depth) const;
    int quiescence(State& currState, SearchLimits search, Nnue& nnue);

    static int get_move_score(const Move& move, const OptionalMove& killer1, const OptionalMove& killer2, const State& currState,
                              const OptionalMove& tt_move, HistoryTable& history, int dsync);

    [[nodiscard]] bool endSearch() const {
        return stop.load(std::memory_order_seq_cst) || timeOut.load(std::memory_order_seq_cst) ||
               forceStop.load(std::memory_order_seq_cst);
    }

    void clearBoolFlags() {
        stop.store(false, std::memory_order_seq_cst);
        timeOut.store(false, std::memory_order_seq_cst);
        forceStop.store(false, std::memory_order_seq_cst);
    }

    void handleRequest(const SearchRequest& request, int& timelimit, int& maxDepth, bool& infinite, std::chrono::time_point<steadyClock> &deadline, std::vector<Move>& possibleMoves);
};

const std::unordered_map<PieceType, int> orderingValue{
    {PieceType::Pawn, 100}, {PieceType::Knight, 300}, {PieceType::Bishop, 325},
    {PieceType::Rook, 500}, {PieceType::Queen, 900},  {PieceType::King, 1300},
};

//
// Created by Kiril on 2025-08-27.
#pragma once
#include <atomic>
#include <unordered_map>

#include "ModuleOnly/EngineOptions.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include "ModuleOnly/TranspositionTable.hpp"
#include "State.hpp"
#include <optional>
#include <vector>

#include "ModuleOnly/LazySmpThreads.hpp"

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
    void stopSearch() {
        forceStop.store(true, std::memory_order_seq_cst);
    }

private:
    State state{};
    Transposition::TranspositionTable transPosTable{};
    OrderingInfo orderInfo{};

    LazySmpThreads lazySmpThreads;
    std::atomic<bool> stop = false;
    std::atomic<bool> timeOut = false;
    std::atomic<bool> forceStop = false;

    Move root(State& currState, const std::vector<Move>& rootMoves, SearchLimits search, OrderingInfo& orderingInfo, RngInfo& rng,
              int& scoreOut);
    int negamax(State& currState, SearchLimits search, OrderingInfo& orderingInfo, RngInfo& rng);
    int quiescence(State& state, SearchLimits search);

    static int get_move_score(const Move& move, const OptionalMove& killer1, const OptionalMove& killer2, const State& currState,
                              const OptionalMove& tt_move, HistoryTable& history, RngInfo& rng);

    [[nodiscard]] bool endSearch() const {
        return stop.load(std::memory_order_seq_cst) || timeOut.load(std::memory_order_seq_cst) ||
               forceStop.load(std::memory_order_seq_cst);
    }
};

const std::unordered_map<PieceType, int> orderingValue{
    {PieceType::Pawn, 100}, {PieceType::Knight, 300}, {PieceType::Bishop, 325},
    {PieceType::Rook, 500}, {PieceType::Queen, 900},  {PieceType::King, 1300},
};

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

constexpr int ENGINE_DEFAULT_THREAD_COUNT = 4;
constexpr int ENGINE_DEFAULT_SEED = 13214;
class Engine {
public:
    Engine()
        : lazySmpThreads(ENGINE_DEFAULT_THREAD_COUNT, ENGINE_DEFAULT_SEED, orderInfo) {}
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
        lazySmpThreads.syncAccumulator(mainNnue);
    }
    void stopSearch() {
        stopFlag.store(true, std::memory_order_seq_cst);
    }

private:
    State state{};
    Transposition::TranspositionTable transPosTable{};
    OrderingInfo orderInfo{};
    const LMRLookUp lmrTable{};

    LazySmpThreads lazySmpThreads;
    std::atomic<bool> stopFlag = false;

    Nnue mainNnue = {};

    Move iterativeDeepening(
        State& currState, const std::vector<Move>& moves, OrderingInfo& orderingInfo,
        RngInfo& rng, Nnue& nnue, bool infinite, int maxDepth, std::chrono::time_point<steadyClock> deadline, int expectedCenter
    );
    std::pair<Move, int> searchMoves(
        State& currState, const std::vector<Move>& rootMoves, SearchLimits search, OrderingInfo& orderingInfo, RngInfo& rng, Nnue& nnue
    );
    int calculateLMRDepth(const State& currState, const Move& move, int i, int depth) const;
    int quiescence(State& currState, SearchLimits search, Nnue& nnue, OrderingInfo& orderingInfo, RngInfo& rng);

    [[nodiscard]] bool endSearch() const {
        return stopFlag.load(std::memory_order_relaxed);
    }

    bool stopSearch(const steadyClock::time_point deadline) {
        if (steadyClock::now() >= deadline) {
            stopFlag.store(true, std::memory_order_relaxed);
            return true;
        }
        return false;
    }

    void clearBoolFlags() {
        stopFlag.store(false, std::memory_order_seq_cst);
    }

    void handleRequest(const SearchRequest& request, int& timelimit, int& maxDepth, bool& infinite, std::chrono::time_point<steadyClock> &deadline, std::vector<Move>& possibleMoves);
};
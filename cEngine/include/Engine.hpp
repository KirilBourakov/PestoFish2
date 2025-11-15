//
// Created by Kiril on 2025-08-27.
#pragma once
#include <atomic>
#include <unordered_map>

#include "ModuleOnly/CastleUtils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include "ModuleOnly/TranspositionTable.hpp"
#include "State.hpp"
#include <array>
#include <optional>
#include <vector>

class HistoryTable {
public:
    int& operator[](const Move& move) {
        return history[move.start.y * BOARD_SIZE + move.start.x][move.end.y * BOARD_SIZE + move.end.x];
    }

private:
    std::array<std::array<int, BOARD_SIZE * BOARD_SIZE>, BOARD_SIZE * BOARD_SIZE> history{};
};

constexpr int maxPly = 128;
class KillerMoves {
public:
    void insert(const int ply, const Move& move) {
        moves[ply][1] = moves[ply][0];
        real[ply][1] = real[ply][0];

        moves[ply][0] = move;
        real[ply][0] = true;
    }
    OptionalMove getFirst(const int ply) {
        if (real[ply][0]) {
            return moves[ply][0];
        }
        return std::nullopt;
    }
    OptionalMove getSecond(const int ply) {
        if (real[ply][1]) {
            return moves[ply][1];
        }
        return std::nullopt;
    }

    void print(Move mv) {
        std::cout << "----- KillerMoves -----" << std::endl;
        for (int i = 0; i < maxPly; i++) {
            for (int j = 0; j < 2; j++) {
                if (real[i][j]) {
                    std::cout << moves[i][j] << " ";
                    if (moves[i][j] == mv) {
                        std::cout << "PLAYING THIS!";
                    }
                }
            }

            std::cout << std::endl;
            if (!real[i][0] && !real[i][1]) {
                break;
            }
        }
        std::cout << "----- End KillerMoves -----" << std::endl;
    }

private:
    std::array<std::array<Move, 2>, maxPly> moves{};
    std::array<std::array<bool, 2>, maxPly> real{};
};

using steadyClock = std::chrono::steady_clock;

struct SearchLimits {
    int color;
    int depth;
    int alpha;
    int beta;
    int ply;

    const steadyClock::time_point& deadline;

    [[nodiscard]] SearchLimits nextLimit() const {
        return {-color, depth - 1, -beta, -alpha, ply + 1, deadline};
    }

    [[nodiscard]] SearchLimits nextLimit(const int searchDepth) const {
        return {-color, searchDepth, -beta, -alpha, ply + 1, deadline};
    }
    [[nodiscard]] SearchLimits nextPVS(const int searchDepth) const {
        return {-color, searchDepth, -alpha - 1, -alpha, ply + 1, deadline};
    }
};

struct OrderingInfo {
    HistoryTable& history;
    KillerMoves& killer;
};

struct RngInfo {
    int seed;
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

    int random() {
        return dist(rng);
    }

    static RngInfo fromSeed(const int seed) {
        const std::mt19937 rng(seed);
        const std::uniform_int_distribution<int> dist(0, 10);
        return {seed, rng, dist};
    }
};

class Engine {
public:
    Move getBestMove();
    void makeEngineMove();

    State& getState() {
        return state;
    };
    void setState(const State& state) {
        this->state = state;
    };
    void forceTimeout() {
        timeOut.store(true, std::memory_order_seq_cst);
    }

private:
    State state{};
    Transposition::TranspositionTable transPosTable{};
    HistoryTable globalHistory{};
    std::atomic<bool> stop = false;
    std::atomic<bool> timeOut = false;

    Move root(State& currState, const std::vector<Move>& rootMoves, SearchLimits search, OrderingInfo& orderingInfo, int& scoreOut,
              RngInfo& rng);
    int negamax(State& currState, SearchLimits search, OrderingInfo& orderingInfo, RngInfo& rng);
    int quiescence(State& state, SearchLimits search);

    static int get_move_score(const Move& move, const OptionalMove& killer1, const OptionalMove& killer2, const State& currState,
                              const OptionalMove& tt_move, HistoryTable& history, RngInfo& rng);

    [[nodiscard]] bool endSearch() const {
        return stop.load(std::memory_order_seq_cst) || timeOut.load(std::memory_order_seq_cst);
    }
};

const std::unordered_map<PieceType, int> orderingValue{
    {PieceType::Pawn, 100}, {PieceType::Knight, 300}, {PieceType::Bishop, 325},
    {PieceType::Rook, 500}, {PieceType::Queen, 900},  {PieceType::King, 1300},
};

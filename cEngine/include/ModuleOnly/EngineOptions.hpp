//
// Created by Kiril on 2025-11-18.
//

#pragma once
#include <chrono>
#include <random>

#include "Move.hpp"
#include "NewBoard.hpp"

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
        moves.at(ply)[1] = moves.at(ply)[0];
        real.at(ply)[1] = real.at(ply)[0];

        moves.at(ply)[0] = move;
        real.at(ply)[0] = true;
    }
    OptionalMove getFirst(const int ply) {
        if (real.at(ply)[0]) {
            return moves[ply][0];
        }
        return std::nullopt;
    }
    OptionalMove getSecond(const int ply) {
        if (real.at(ply)[1]) {
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

constexpr int MAX_DEPTH = 64;
constexpr int MAX_MOVES = 64;
class LMRLookUp {
public:
    LMRLookUp() {
        reductions[0].fill(0);

        for (int d = 1; d < MAX_DEPTH; d++) {
            for (int m = 1; m < MAX_MOVES; m++) {
                const double reduction = 0.99 + (std::log(d) * std::log(m)) / 3.14;
                reductions[d][m] = static_cast<int>(reduction);
            }
        }
    }

    int operator()(int depth, int index) const noexcept {
        depth = std::min(MAX_DEPTH-1, depth);
        index = std::min(MAX_MOVES-1, index);
        return reductions[depth][index];
    }

private:
    std::array<std::array<int, MAX_MOVES>, MAX_DEPTH> reductions;
};

using steadyClock = std::chrono::steady_clock;

struct SearchLimits {
    int color;
    int depth;
    int alpha;
    int beta;
    int ply;
    steadyClock::time_point deadline;

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
    HistoryTable history;
    KillerMoves killer;
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

struct SearchRequest {
    std::optional<std::vector<Move>> searchmoves = std::nullopt;
    int wtime = -1;
    int btime = -1;
    int winc = 0;
    int binc = 0;
    int movestogo = -1;
    int depth = -1;
    int movetime = -1;
    bool infinite = false;
};

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

class KillerMoves {
public:
    explicit KillerMoves(const int depth)
        : moves(depth)
        , real(depth) {}

    void insert(const int depth, const Move& move) {
        moves[depth][1] = moves[depth][0];
        real[depth][1] = real[depth][0];

        moves[depth][0] = move;
        real[depth][0] = true;
    }
    std::optional<Move> getFirst(const int depth) {
        if (real[depth][0]) {
            return moves[depth][0];
        }
        return std::nullopt;
    }
    std::optional<Move> getSecond(const int depth) {
        if (real[depth][1]) {
            return moves[depth][1];
        }
        return std::nullopt;
    }

private:
    std::vector<std::array<Move, 2>> moves;
    std::vector<std::array<bool, 2>> real;
};

using steadyClock = std::chrono::steady_clock;
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

private:
    State state{};
    Transposition::TranspositionTable transPosTable{};
    HistoryTable globalHistory{};
    std::atomic<bool> stop = false;
    std::atomic<bool> timeOut = false;

    Move root(State& currState, const std::vector<Move>& rootMoves, int depth, int alpha, int beta, HistoryTable& history,
              int& scoreOut, int seed, const steadyClock::time_point& deadline);
    int negamax(State& currState, int depth, int alpha, int beta, int colorRep, HistoryTable& history, std::mt19937& rng,
                std::uniform_int_distribution<int>& dist, KillerMoves& killerMoves, const steadyClock::time_point& deadline);
    int quiescence(State& state, int colorRep, int depth, int alpha, int beta, const steadyClock::time_point& deadline);
    static int get_move_score(const Move& move, const std::optional<Move>& killer1, const std::optional<Move>& killer2,
                              const State& currState, const std::optional<Move>& tt_move, HistoryTable& history, std::mt19937& rng,
                              std::uniform_int_distribution<int>& dist);

    [[nodiscard]] bool endSearch() const {
        return stop.load(std::memory_order_seq_cst) || timeOut.load(std::memory_order_seq_cst);
    }
};

const std::unordered_map<PieceType, int> orderingValue{
    {PieceType::Pawn, 100}, {PieceType::Knight, 300}, {PieceType::Bishop, 325},
    {PieceType::Rook, 500}, {PieceType::Queen, 900},  {PieceType::King, 1300},
};

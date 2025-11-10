//
// Created by Kiril on 2025-08-27.
#pragma once
#include <atomic>
#include <unordered_map>

#include "ModuleOnly/Board.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/TranspositionTable.hpp"
#include "Move.hpp"
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

    Move root(State& currState, const std::vector<Move>& rootMoves, const int depth, int& alpha, int& beta, HistoryTable& history,
              int& scoreOut, int seed);
    int minimax(State& currState, int curr_depth, int max_depth, int alpha, int beta, HistoryTable& history, std::mt19937& rng,
                std::uniform_int_distribution<int>& dist);
    int quiescence(State& state, int depth, int alpha, int beta);
    static int get_move_score(const Move& move, const State& currState, const std::optional<Move>& tt_move, HistoryTable& history,
                              std::mt19937& rng, std::uniform_int_distribution<int>& dist);
};

const std::unordered_map<PieceType, int> orderingValue{
    {PieceType::Pawn, 100}, {PieceType::Knight, 300}, {PieceType::Bishop, 325},
    {PieceType::Rook, 500}, {PieceType::Queen, 900},  {PieceType::King, 1300},
};

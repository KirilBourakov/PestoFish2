//
// Created by Kiril on 2025-08-27.
//
module;
#include <unordered_map>

export module Engine;
import <optional>;
import <vector>;
import <array>;
import Board;
import Enums;
import State;
import Move;
import Transposition;

class HistoryTable {
public:
    int& operator[](const Move& move) {
        return history[move.start.y * BOARD_SIZE + move.start.x][move.end.y * BOARD_SIZE + move.end.x];
    }

private:
    std::array<std::array<int, BOARD_SIZE * BOARD_SIZE>, BOARD_SIZE * BOARD_SIZE> history{};
};

export class Engine {
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

    Move root(const std::vector<Move>& rootMoves, const int depth, int& alpha, int& beta, HistoryTable& history);
    int minimax(State& state, int curr_depth, int max_depth, int alpha, int beta, HistoryTable& history);
    static int get_move_score(const Move& move, const State& state, const std::optional<Move>& tt_move, HistoryTable& history);
};

const std::unordered_map<PieceType, int> orderingValue{
    {PieceType::Pawn, 100}, {PieceType::Knight, 300}, {PieceType::Bishop, 325},
    {PieceType::Rook, 500}, {PieceType::Queen, 900},  {PieceType::King, 1300},
};

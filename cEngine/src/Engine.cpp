//
// Created by Kiril on 2025-08-23.
//
module;
#include <algorithm>
#include <execution>
#include <iostream>
#include <mutex>
#include <ostream>

module Engine;
import Move;
import State;
import Evaluator;

void Engine::makeEngineMove() {
    state.makeMove(getBestMove());
}

// TODO: implement ply so earlier mates are more prioritized
Move Engine::getBestMove() {
    int alpha = -INF;
    int beta = INF;
    Color rootColor = state.getActiveColor();

    std::optional<Move> bestMove = std::nullopt;
    int bestEval = (rootColor == Color::White) ? -INF : INF;

    Transposition::Entry entry_out;
    transPosTable.lookup(state.getZobrist(), entry_out);

    std::vector<Move> possibleMoves = state.getMoves();
    std::ranges::sort(possibleMoves, [this, &entry_out](const Move& a, const Move& b) {
        return get_move_score(a, this->state, entry_out.bestMove, this->globalHistory) >
               get_move_score(b, this->state, entry_out.bestMove, this->globalHistory);
    });

    for (int max_depth = 1; max_depth <= 3; max_depth++) {
        for (Move move : possibleMoves) {
            state.makeMove(move);
            int eval = minimax(state, 0, max_depth, alpha, beta, globalHistory);
            state.undoMove();

            if (!bestMove.has_value() || Evaluator::isBetterEval(rootColor, bestEval, eval)) {
                bestMove = move;
                bestEval = eval;
            }

            if (rootColor == Color::White) {
                alpha = std::max(alpha, eval);
            } else {
                beta = std::min(beta, eval);
            }

            if (beta <= alpha) {
                break;
            }
        }
    }
    return bestMove.value();
}

int Engine::minimax(State& state, int curr_depth, int max_depth, int alpha, int beta, HistoryTable& history) {
    // --- Probe Transpose Table ---
    const uint64_t zobrist = state.getZobrist();

    Transposition::Entry entry_out;
    if (transPosTable.lookup(zobrist, entry_out)) {
        if (entry_out.depth == max_depth) {
            switch (entry_out.cutoffType) {
            case Transposition::CutoffType::EXACT:
                return entry_out.score;
            case Transposition::CutoffType::LOWER_BOUND:
                alpha = std::max(alpha, static_cast<int>(entry_out.score));
                break;
            case Transposition::CutoffType::UPPER_BOUND:
                beta = std::min(beta, static_cast<int>(entry_out.score));
                break;
            default:
                throw std::invalid_argument("Invalid cutoff type");
            }
            if (alpha >= beta) {
                return entry_out.score;
            }
        }
    }

    // --- Check we should stop ---
    if (curr_depth == max_depth) {
        return Evaluator::evaluate(state);
    }

    std::vector<Move> possibleMoves = state.getMoves();
    GameState currGameState = state.getGameState(possibleMoves);
    if (currGameState == GameState::DRAW || currGameState == GameState::STALEMATE) {
        return 0;
    }
    if (currGameState == GameState::BLACK_WIN) {
        return -MATE_SCORE;
    }
    if (currGameState == GameState::WHITE_WIN) {
        return MATE_SCORE;
    }

    std::ranges::sort(possibleMoves, [state, &entry_out, &history](const Move& a, const Move& b) {
        return get_move_score(a, state, entry_out.bestMove, history) > get_move_score(b, state, entry_out.bestMove, history);
    });

    // --- Run a layer of minimax ---
    int bestEval = (state.getActiveColor() == Color::White) ? -INF : INF;
    Move bestMove;

    bool cutoffOccurred = false;
    const int alphaOrig = alpha;
    for (const Move move : possibleMoves) {
        state.makeMove(move);
        int eval = minimax(state, curr_depth + 1, max_depth, alpha, beta, history);
        state.undoMove();

        if (Evaluator::isBetterEval(state.getActiveColor(), bestEval, eval)) {
            bestEval = eval;
            bestMove = move;
        }

        // alpha beta pruning
        if (state.getActiveColor() == Color::White) {
            alpha = std::max(alpha, eval);
        } else {
            beta = std::min(beta, eval);
        }
        if (beta <= alpha) {
            cutoffOccurred = true;
            break;
        }
    }

    // minimizing has a way to get beta, and this is greater then that, so they
    // cut
    Transposition::CutoffType cutoffType;
    if (cutoffOccurred) {
        history[bestMove] = (max_depth - curr_depth) * (max_depth - curr_depth);
        cutoffType = Transposition::CutoffType::LOWER_BOUND;
    }
    // maximizing has a way to get alphaOrig, and this path is worse then that,
    // so we cut
    else if (bestEval <= alphaOrig) {
        cutoffType = Transposition::CutoffType::UPPER_BOUND;
    } else {
        cutoffType = Transposition::CutoffType::EXACT;
    }

    // using full move clock for age
    transPosTable.insert(state.getZobrist(), bestMove, max_depth - curr_depth, bestEval, cutoffType, state.getFullMoveClock());
    return bestEval;
}

int Engine::get_move_score(const Move& move, const State& state, const std::optional<Move>& tt_move, HistoryTable& history) {
    if (tt_move.has_value() && move == tt_move.value()) {
        return 1000000;
    }
    if (state.getAt(move.end) != Pieces::EMPTY) {
        const int victim_value = orderingValue.at(Pieces::piece_type(state.getAt(move.end)));
        const int attacker_value = orderingValue.at(Pieces::piece_type(state.getAt(move.start)));
        return 500000 + victim_value * 10 - attacker_value;
    }
    if (move.enPassantCapture) {
        return 500000;
    }
    return history[move];
}

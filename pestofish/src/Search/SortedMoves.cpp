//
// Created by Kiril on 1/28/2026.
//

#include "pestofish/Search/SortedMoves.hpp"


static const int orderingValue[] = {
    0, 100, 300, 325, 500, 900, 1300
};

Move& SortedMoves::next() {
    int best_index = sorted;
    int best_score = scores[best_index];
    for (int i = sorted+1; i < moves.size(); i++) {
        if (scores[i] > best_score) {
            best_index = i;
            best_score = scores[i];
        }
    }

    std::swap(moves[sorted], moves[best_index]);
    std::swap(scores[sorted], scores[best_index]);
    sorted++;
    return moves[sorted - 1];
}

int SortedMoves::get_move_score(const Move& move, const OptionalMove& killer1, const OptionalMove& killer2, const State& currState,
                           const OptionalMove& tt_move, HistoryTable& history) {
    if (tt_move.has_value() && move == tt_move.value()) {
        return 1000000;
    }
    if ((killer1.has_value() && move == killer1.value()) || (killer2.has_value() && move == killer2.value())) {
        return 900000;
    }

    // Cheap way to move promotions up above non promotions
    int mod = 0;
    if (move.getPromotedTo().has_value()) {
        const int promoteValue = orderingValue[static_cast<int>(Pieces::piece_type(move.getPromotedTo().value()))];
        mod += promoteValue;
    }

    if (currState.getAt(move.getEnd()) != Pieces::EMPTY) {
        const int victim_value = orderingValue[static_cast<int>(Pieces::piece_type(currState.getAt(move.getEnd())))];
        const int attacker_value = orderingValue[static_cast<int>(Pieces::piece_type(currState.getAt(move.getStart())))];
        return 500000 + victim_value * 10 - attacker_value + mod;
    }
    if (move.getEnPassantCapture()) {
        return 500000 + mod;
    }
    return history[move] + mod;
}

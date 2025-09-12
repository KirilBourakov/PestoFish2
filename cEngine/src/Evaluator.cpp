//
// Created by Kiril on 2025-09-11.
//

module Evaluator;
import Board;
import Types;

double Evaluate::evaluate(const State &state) {
    double eval = 0;
    // count pieces
    ///TODO: replace with real eval
    for (int y = 0; y < BOARD_SIZE; y++) {
       for (int x = 0; x < BOARD_SIZE; x++) {
           if (sameColor(BLACK, state.getBoard()[y][x])) {
               eval -= 1.0;
           }
           else if (sameColor(WHITE, state.getBoard()[y][x])) {
               eval += 1.0;
           }
       }
    }
    return eval;
}
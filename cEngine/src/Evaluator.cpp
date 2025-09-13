//
// Created by Kiril on 2025-09-11.
//

module Evaluator;
import Board;
import Types;

double Evaluator::evaluate(const State &state) {
    double eval = 0;
    // count pieces
    ///TODO: replace with real eval
    for (int y = 0; y < BOARD_SIZE; y++) {
       for (int x = 0; x < BOARD_SIZE; x++) {
           const int dir = sameColor(WHITE, state.getBoard()[y][x]) ? -1 : 1;
           double change;

           switch (std::abs((state.getBoard()[y][x]))) {
               case WHITE_PAWN:
                   change = 1;
                   break;
               case WHITE_KNIGHT:
                   change = 2;
                   break;
               case WHITE_BISHOP:
                   change = 2.25;
                   break;
               case WHITE_ROOK:
                   change = 5;
                   break;
               case WHITE_QUEEN:
                   change = 9;
                   break;
               default: change = 0;
           }
           eval += dir * change;
       }
    }
    return eval;
}

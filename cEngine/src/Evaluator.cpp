//
// Created by Kiril on 2025-09-11.
//
module;
#include <cmath>

module Evaluator;
import Board;
import Types;
import PieceSqTables;

double Evaluator::evaluate(const State &state) {
    bool endgame = false;
    int eval = getSquareWiseEvalAndGamePhase(state, endgame);

    return eval;
}

/**
 * This gets the squarewise eval, and the game phase
 * @param state the game state
 * @param endgame pointed to game phase indicator.
 * Set to true if both players have <= 13 points of material, and false otherwise, via Speelman's definition of endgame
 * @return squarewise eval, defined as eval that can be calculated by looking at a single square
 */
int Evaluator::getSquareWiseEvalAndGamePhase(const State &state, bool& endgame) {
    int middleMaterialCount = 0;
    int endMaterialCount = 0;
    int whiteNormalCount = 0;
    int blackNormalCount = 0;

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            auto currPiece = static_cast<Piece>(abs(state.getBoard()[y][x]));
            if (currPiece != EMPTY) {
                const int dir = sameColor(WHITE, state.getBoard()[y][x]) ? 1 : -1;

                // Raw material (which king does not have)
                if (currPiece != WHITE_KING) {
                    middleMaterialCount += middleGameScore.at(currPiece) * dir;
                    endMaterialCount += endGameScore.at(currPiece) * dir;
                }

                // piece square
                middleMaterialCount += mg_table.at(currPiece)[y][x] * dir;
                endMaterialCount += eg_table.at(currPiece)[y][x] * dir;

                // tracking to determine if it is an end game
                if (sameColor(WHITE, currPiece)) {
                    whiteNormalCount += normalScore.contains(currPiece) ? normalScore.at(currPiece) * dir : 0;
                } else if (sameColor(BLACK, currPiece)) {
                    blackNormalCount += normalScore.contains(currPiece) ? normalScore.at(currPiece) * dir : 0;
                }
            }
        }
    }

    if (whiteNormalCount <= 13 && blackNormalCount <= 13) {
        endgame = true;
        return endMaterialCount;
    }
    endgame = false;
    return middleMaterialCount;
}

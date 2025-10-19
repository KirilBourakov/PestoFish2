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
            if (const Piece currPiece = state.getBoard()[y][x]; currPiece != EMPTY) {
                const int dir = sameColor(WHITE, currPiece) ? 1 : -1;
                const auto colorLessPiece = static_cast<Piece>(abs(currPiece));
                // Raw material (which king does not have)
                if (colorLessPiece != WHITE_KING) {
                    middleMaterialCount += middleGameScore.at(colorLessPiece) * dir;
                    endMaterialCount += endGameScore.at(colorLessPiece) * dir;
                }

                // piece square
                middleMaterialCount += mg_table.at(colorLessPiece)[y][x] * dir;
                endMaterialCount += eg_table.at(colorLessPiece)[y][x] * dir;

                // mobility (for now, lets just say each possible move is worth 3 centipawns)
                std::vector<Move> moves;
                state.addMoves(x, y, moves);
                middleMaterialCount += 3 * static_cast<int>(moves.size()) * dir;
                endMaterialCount +=  3 * static_cast<int>(moves.size()) * dir;

                // POSSIBLE Evaluation methods:
                // pieces on enemy half of the board

                // tracking to determine if it is an end game
                if (sameColor(WHITE, currPiece)) {
                    whiteNormalCount += normalScore.contains(colorLessPiece) ? normalScore.at(colorLessPiece) * dir : 0;
                } else if (sameColor(BLACK, currPiece)) {
                    blackNormalCount += normalScore.contains(colorLessPiece) ? normalScore.at(colorLessPiece) * dir : 0;
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

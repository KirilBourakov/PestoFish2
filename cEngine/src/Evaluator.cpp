//
// Created by Kiril on 2025-09-11.
//
#include <cmath>
#include <iostream>

#include "Evaluator.hpp"

#include "ModuleOnly/Board.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/PieceSqTables.hpp"
#include "Move.hpp"

bool Evaluator::isBetterEval(const Color color, const int currBest, const int value) {
    if (color == Color::White) {
        return value > currBest;
    }
    return value < currBest;
}

int Evaluator::evaluate(const State& state) {
    bool endgame = false;
    int eval = getSquareWiseEvalAndGamePhase(state, endgame);

    return eval;
}

/**
 * This gets the squarewise eval, and the game phase
 * @param state the game state
 * @param endgame pointed to game phase indicator.
 * Set to true if both players have <= 13 points of material, and false
 * otherwise, via Speelman's definition of endgame
 * @return squarewise eval, defined as eval that can be calculated by looking
 * at a single square
 */
int Evaluator::getSquareWiseEvalAndGamePhase(const State& state, bool& endgame) {
    int middleMaterialCount = 0;
    int endMaterialCount = 0;
    int whiteNormalCount = 0;
    int blackNormalCount = 0;

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (const Pieces::Piece currPiece = state.getBoard()[y][x]; currPiece != Pieces::EMPTY) {
                const int dir = Pieces::sameColor(Color::White, currPiece) ? 1 : -1;
                const auto colorLessPiece = Pieces::piece_type(currPiece);
                // Raw material (which king does not have)
                if (colorLessPiece != PieceType::King) {
                    middleMaterialCount += middleGameScore.at(colorLessPiece) * dir;
                    endMaterialCount += endGameScore.at(colorLessPiece) * dir;
                }

                // piece square
                middleMaterialCount += mg_table.at(colorLessPiece)[y][x] * dir;
                endMaterialCount += eg_table.at(colorLessPiece)[y][x] * dir;

                // mobility (for now, lets just say each possible move is worth 3
                // centipawns)
                // TODO: rework, too slow
                // std::vector<Move> moves;
                // state.addMoves(x, y, moves);
                // middleMaterialCount += 3 * static_cast<int>(moves.size()) * dir;
                // endMaterialCount += 3 * static_cast<int>(moves.size()) * dir;

                // POSSIBLE Evaluation methods:
                // pieces on enemy half of the board

                // tracking to determine if it is an end game
                if (Pieces::sameColor(Color::White, currPiece)) {
                    whiteNormalCount += normalScore.contains(colorLessPiece) ? normalScore.at(colorLessPiece) * dir : 0;
                } else if (Pieces::sameColor(Color::Black, currPiece)) {
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

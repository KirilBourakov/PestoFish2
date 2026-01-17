//
// Created by Kiril on 2025-09-11.
//
#include <cmath>
#include <iostream>

#include "pestofish/Eval/Evaluator.hpp"

#include "pestofish/Core/Utils.hpp"
#include "pestofish/Core/Enums.hpp"
#include "pestofish/Core/PieceSqTables.hpp"

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
            if (const Pieces::Piece currPiece = state.getBoard().at(y, x); currPiece != Pieces::EMPTY) {
                const int dir = Pieces::sameColor(Color::White, currPiece) ? 1 : -1;
                const auto pieceType = Pieces::piece_type(currPiece);
                // Raw material (which king does not have)
                if (pieceType != PieceType::King) {
                    middleMaterialCount += middleGameScore.at(pieceType) * dir;
                    endMaterialCount += endGameScore.at(pieceType) * dir;
                }

                // piece square
                if (Pieces::sameColor(Color::Black, currPiece)) {
                    middleMaterialCount += -mg_table.at(pieceType)[7 - y][x];
                    endMaterialCount += -eg_table.at(pieceType)[7 - y][x];
                } else {
                    middleMaterialCount += mg_table.at(pieceType)[y][x];
                    endMaterialCount += eg_table.at(pieceType)[y][x];
                }

                // tracking to determine if it is an end game
                if (Pieces::sameColor(Color::White, currPiece)) {
                    whiteNormalCount += normalScore.contains(pieceType) ? normalScore.at(pieceType) : 0;
                } else if (Pieces::sameColor(Color::Black, currPiece)) {
                    blackNormalCount += normalScore.contains(pieceType) ? normalScore.at(pieceType) : 0;
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

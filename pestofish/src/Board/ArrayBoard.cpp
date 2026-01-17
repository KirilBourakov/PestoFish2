//
// Created by Kiril on 2025-08-23.
//
#include <stdexcept>

#include "pestofish/ModuleOnly/Utils.hpp"
#include "pestofish/ModuleOnly/Enums.hpp"
#include "pestofish/ModuleOnly/Move.hpp"
#include "pestofish/Board/ArrayBoard.hpp"

#include <optional>
#include <vector>

using moveSet = std::vector<std::pair<int, int>>;

bool ArrayBoard::isDrawFromMaterial() const {
    int wBishops = 0;
    int wKnights = 0;
    int bBishops = 0;
    int bKnights = 0;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            switch (board[y][x]) {
                case Pieces::WHITE_PAWN:
                case Pieces::BLACK_PAWN:
                case Pieces::WHITE_ROOK:
                case Pieces::BLACK_ROOK:
                case Pieces::WHITE_QUEEN:
                case Pieces::BLACK_QUEEN:
                    return false;

                case Pieces::WHITE_KNIGHT:
                    wKnights++;
                    break;
                case Pieces::WHITE_BISHOP:
                    wBishops++;
                    break;
                case Pieces::BLACK_KNIGHT:
                    bKnights++;
                    break;
                case Pieces::BLACK_BISHOP:
                    bBishops++;
                    break;

                case Pieces::WHITE_KING:
                case Pieces::BLACK_KING:
                case Pieces::EMPTY:
                    break;
            }
        }
    }
    const bool whiteCanWin = (wBishops >= 1 && wKnights >= 1) || (wBishops >= 2) || (wKnights >= 3);
    const bool blackCanWin = (bBishops >= 1 && bKnights >= 1) || (bBishops >= 2) || (bKnights >= 3);
    return !whiteCanWin && !blackCanWin;
}

std::ostream& operator<<(std::ostream& os, const ArrayBoard& b) {
    os << "  +-----------------+\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        os << i + 1 << " | ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            os << Pieces::piece_char(b.board[i][j]) << ' ';
        }
        os << "|\n";
    }
    os << "  +-----------------+\n";
    return os;
}
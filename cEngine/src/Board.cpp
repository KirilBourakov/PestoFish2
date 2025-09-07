//
// Created by Kiril on 2025-08-23.
//

module Board;
import Types;

BoardArray getStartingBoard() {
    return BoardArray{{
        {{BLACK_ROOK,   BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
        {{BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN,  BLACK_PAWN, BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,  WHITE_PAWN, WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN}},
        {{WHITE_ROOK,   WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}
    }};
}

bool inBounds(const int x) {
    return x >= 0 && x < BOARD_SIZE;
}
bool inBounds(const int x, const int y) {
    return inBounds(x) && inBounds(y);
}

bool sameColor(const Color color, const Piece piece) {
    return (color == WHITE && piece > 0) || (color == BLACK && piece < 0);
}
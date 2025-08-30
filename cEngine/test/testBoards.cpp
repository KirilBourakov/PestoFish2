//
// Created by Kiril on 2025-08-26.
//
module testBoard;
import Types;


BoardArray foolsMatePosition() {
    return BoardArray{{
        {{BLACK_ROOK,   BLACK_KNIGHT, BLACK_BISHOP, EMPTY,       BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK}},
        {{BLACK_PAWN,   EMPTY,        BLACK_PAWN,   BLACK_PAWN,  EMPTY,      BLACK_PAWN,   BLACK_PAWN,   BLACK_PAWN}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_QUEEN,EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        BLACK_PAWN,   EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        WHITE_PAWN}},
        {{EMPTY,        EMPTY,        WHITE_PAWN,   EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        WHITE_PAWN,  EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   WHITE_PAWN,   EMPTY,        EMPTY,       WHITE_PAWN, WHITE_PAWN,   WHITE_PAWN,   EMPTY}},
        {{WHITE_ROOK,   WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK}}
    }};
}

BoardArray smotheredMatePosition() {
    return BoardArray{{
        {{BLACK_ROOK,   EMPTY,        EMPTY,        BLACK_BISHOP,BLACK_KING, BLACK_KNIGHT, EMPTY,        BLACK_ROOK}},
        {{BLACK_PAWN,   BLACK_PAWN,   EMPTY,        BLACK_ROOK,  BLACK_PAWN, BLACK_ROOK,   BLACK_PAWN,   BLACK_PAWN}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        BLACK_ROOK,   EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       WHITE_KNIGHT,EMPTY,       EMPTY,        EMPTY}},
        {{WHITE_KNIGHT, EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        WHITE_PAWN,   EMPTY}},
        {{WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,   WHITE_PAWN,  EMPTY,      WHITE_PAWN,   EMPTY,        WHITE_PAWN}},
        {{WHITE_ROOK,   EMPTY,        WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, EMPTY,        EMPTY,        WHITE_ROOK}}
    }};
}

BoardArray unBlockedCastle() {
    return BoardArray{{
        {{BLACK_ROOK,   EMPTY,        EMPTY,        EMPTY,       BLACK_KING, EMPTY,        EMPTY,        BLACK_ROOK}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        WHITE_PAWN,  WHITE_PAWN, WHITE_PAWN,        EMPTY,        EMPTY}},
        {{WHITE_ROOK,   EMPTY,        EMPTY,        EMPTY,       WHITE_KING, EMPTY,        EMPTY,        WHITE_ROOK}}
    }};
}

BoardArray simpleMiddleGame() {
    return BoardArray{{
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_KING, EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        BLACK_ROOK}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_QUEEN, EMPTY,       EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       WHITE_PAWN, EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        WHITE_ROOK,   EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        WHITE_QUEEN, WHITE_KING, EMPTY,        EMPTY,        EMPTY}}
    }};
}

BoardArray knightCheck() {
    return BoardArray{{
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_KING, EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        BLACK_ROOK}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_QUEEN, WHITE_KNIGHT,EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       WHITE_PAWN, EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        WHITE_ROOK,   EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        BLACK_KNIGHT,EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        WHITE_QUEEN, WHITE_KING, EMPTY,        EMPTY,        EMPTY}}
    }};
}

BoardArray straightCheck() {
    return BoardArray{{
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_KING, EMPTY,        EMPTY,        WHITE_ROOK}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        BLACK_ROOK}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_QUEEN,EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       WHITE_PAWN, EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        BLACK_QUEEN,  EMPTY,       WHITE_KING, EMPTY,        EMPTY,        EMPTY}}
    }};
}

BoardArray diagCheck() {
    return BoardArray{{
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_KING, EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        BLACK_ROOK}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_QUEEN,EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       WHITE_PAWN, EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_BISHOP, EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        BLACK_QUEEN,  EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        WHITE_KING,  EMPTY, EMPTY,        EMPTY,        EMPTY}}
    }};
}

BoardArray pawnCheck() {
    return BoardArray{{
        {{EMPTY,        BLACK_KING,   EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        BLACK_PAWN,   EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        WHITE_KING,   EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}}
    }};
}

BoardArray kingCheck() {
    return BoardArray{{
        {{EMPTY,        BLACK_KING,   EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_KING,   EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}}
    }};
}

BoardArray kingsFlankPawns() {
    return BoardArray{{
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   EMPTY,        EMPTY,        WHITE_KING,  EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        BLACK_KING,   BLACK_PAWN,   EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}}
    }};
}


// UNUSED

BoardArray whiteReadyToPromote() {
    return BoardArray{{
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_KING, EMPTY,        EMPTY,        EMPTY}},
        {{WHITE_PAWN,   EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       WHITE_KING, EMPTY,        EMPTY,        EMPTY}}
    }};
}

BoardArray whiteEnpassent() {
    return BoardArray{{
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_KING, EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        WHITE_PAWN,   BLACK_PAWN,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       WHITE_KING, EMPTY,        EMPTY,        EMPTY}}
    }};
}

BoardArray blackEnpassent() {
    return BoardArray{{
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       BLACK_KING, EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        WHITE_PAWN,   BLACK_PAWN,  EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       EMPTY,      EMPTY,        EMPTY,        EMPTY}},
        {{EMPTY,        EMPTY,        EMPTY,        EMPTY,       WHITE_KING, EMPTY,        EMPTY,        EMPTY}}
    }};
}
//
// Created by Kiril on 2025-08-31.
//
module State;
import Board;
import Utils;
#include <stdexcept>

State::State() {
    board = get_start_board();

    activeColor = WHITE;
    castlingRights = 0b1111;
    enPassantSquare = std::nullopt;
    halfMoveClock = 0;
    fullMoveCock = 0;

    whiteKingSquare = {.x=4, .y=7};
    blackKingSquare = {.x=4, .y=0};
}

void State::makeMove(Move move) {
    const HistoricalEntry entry = {move, castlingRights, enPassantSquare};

    const Piece newPiece = move.promotedTo.value_or(board[move.start.y][move.start.x]);
    if (!sameColor(activeColor, newPiece)) {
        throw std::invalid_argument("Moving piece from wrong side.");
    }

    board[move.end.y][move.end.x] = newPiece;
    board[move.start.y][move.start.x] = EMPTY;
    if (move.enPassantCapture) {
        if (activeColor == WHITE) {
            board[move.end.y+1][move.end.x] = EMPTY;
        }
        else {
            board[move.end.y-1][move.end.x] = EMPTY;
        }
    }
    if (move.newEnPassantSquare.has_value()) {
        enPassantSquare = move.newEnPassantSquare.value();
    }


    // handle removing castling when a rook is taken
    const int backRow = activeColor == WHITE ? 7 : 0;
    const bool movingKing = std::abs(newPiece) == WHITE_KING;

    const bool movingQueenSideRook = backRow == move.start.y && move.start.x == 0;
    const bool capturingQueenSideRook = backRow == move.end.y && move.end.x == 0;
    if ((movingQueenSideRook || capturingQueenSideRook || movingKing) && allowCastle(activeColor, LONG, castlingRights)) {
        disAllowCastle(activeColor, LONG, castlingRights);
    }
    const bool movingKingSideRook = backRow == move.start.y && move.start.x == 7;
    const bool capturingKingSideRook = backRow == move.end.y && move.end.x == 7;
    if ((movingKingSideRook || capturingKingSideRook || movingKing) && allowCastle(activeColor, SHORT, castlingRights)) {
        disAllowCastle(activeColor, SHORT, castlingRights);
    }

    if (move.castle == LONG) {
        const Piece rook = board[move.end.y][0];
        board[move.end.y][0] = EMPTY;
        board[move.end.y][move.end.x+1] = rook;
        disAllowCastle(activeColor, LONG, castlingRights);
    }
    else if (move.castle == SHORT) {
        const Piece rook = board[move.end.y][7];
        board[move.end.y][7] = EMPTY;
        board[move.end.y][move.end.x-1] = rook;
        disAllowCastle(activeColor, SHORT, castlingRights);
    }

    history.push_back(entry);
    activeColor = activeColor == WHITE ? BLACK : WHITE;
}

void State::undoMove() {
    HistoricalEntry entry = history.back();
    Move move = entry.move;
    history.pop_back();
    activeColor = activeColor == WHITE ? BLACK : WHITE;

    const int color = activeColor == WHITE ? 1 : -1;
    const Piece pieceMoved = move.promotedTo.has_value() ? static_cast<Piece>(color * WHITE_PAWN) : board[move.end.y][move.end.x];

    if (move.castle == LONG) {
        board[move.end.y][0] = static_cast<Piece>(color * WHITE_ROOK);
        board[move.end.y][move.end.x+1] = EMPTY;
    }
    else if (move.castle == SHORT) {
        board[move.end.y][7] = static_cast<Piece>(color * WHITE_ROOK);
        board[move.end.y][move.end.x-1] = EMPTY;
    }
    if (move.enPassantCapture) {
        if (activeColor == WHITE) {
            board[move.end.y+1][move.end.x] = BLACK_PAWN;
        }
        else {
            board[move.end.y-1][move.end.x] = WHITE_PAWN;
        }
    }

    board[move.end.y][move.end.x] = move.endPiece;
    board[move.start.y][move.start.x] = pieceMoved;

    castlingRights = entry.castlingBeforeMove;
    enPassantSquare = entry.enPassantBeforeMove;
}

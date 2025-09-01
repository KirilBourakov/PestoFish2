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
    castlingHistory.push_back(castlingRights);

    const Piece newPiece = move.promotedTo.value_or(board[move.start.y][move.start.x]);
    if (!sameColor(activeColor, newPiece)) {
        throw std::invalid_argument("Moving piece from wrong side.");
    }

    board[move.end.y][move.end.x] = newPiece;
    board[move.start.y][move.start.x] = EMPTY;

    if (move.enPassant.has_value()) {
        auto [x, y] = move.enPassant.value();
        board[y][x] = EMPTY;
    }

    // handle removing castling when a rook is taken
    int backRow = activeColor == WHITE ? 7 : 0;
    if (backRow == move.start.y && move.start.x == 0 && allowCastle(activeColor, LONG, castlingRights)) {
        disAllowCastle(activeColor, LONG, castlingRights);
    }
    if (backRow == move.start.y && move.end.x == 7 && allowCastle(activeColor, SHORT, castlingRights)) {
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

    history.push_back(move);
    activeColor = activeColor == WHITE ? BLACK : WHITE;
}

void State::undoMove() {
    Move move = history.back();
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

    if (move.enPassant.has_value()) {
        auto [x, y] = move.enPassant.value();
        board[y][x] = static_cast<Piece>(-1 * color * WHITE_PAWN);
        enPassantSquare = move.enPassant.value();
    }

    board[move.end.y][move.end.x] = move.endPiece;
    board[move.start.y][move.start.x] = pieceMoved;
    castlingRights = castlingHistory.back();
    castlingHistory.pop_back();
}

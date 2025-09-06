//
// Created by Kiril on 2025-08-31.
//
module;
#include <stdexcept>
module State;
import Board;
import Utils;
import Move;


State::State() {
    board = get_start_board();

    activeColor = WHITE;
    castlingRights = 0b1111;
    enPassantSquare = std::nullopt;
    halfMoveClock = 0;
    fullMoveClock = 0;

    whiteKingSquare = {.x=4, .y=7};
    blackKingSquare = {.x=4, .y=0};
}

std::vector<Move> State::getMoves() {
    std::vector<Move> moves;
    // get all pseudo legal moves
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (sameColor(activeColor, board[y][x])) {
                switch (std::abs(board[y][x])) {
                    case WHITE_PAWN:
                        addPawnMoves(board, x, y, activeColor, enPassantSquare, moves);
                        break;
                    case WHITE_KNIGHT:
                        addKnightMoves(board, x, y, activeColor, moves);
                        break;
                    case WHITE_BISHOP:
                        addSlidingMoves(board, x, y, activeColor, false, true, moves);
                        break;
                    case WHITE_ROOK:
                        addSlidingMoves(board, x, y, activeColor, true, false, moves);
                        break;
                    case WHITE_QUEEN:
                        addSlidingMoves(board, x, y, activeColor, true, true, moves);
                        break;
                    case WHITE_KING:
                        addKingMoves(board, x, y, activeColor, castlingRights, moves);
                        break;
                    default: throw std::invalid_argument("Invalid piece");
                }
            }
        }
    }

    std::vector<Move> legalMoves;
    for (Move move : moves) {
        bool isValid = true;
        if (move.castle == LONG) {
            isValid = !isAttacked(board, BoardPosition{.x=move.end.x+1, .y=move.end.y}) && !isAttacked(board, BoardPosition{.x=move.end.x+2, .y=move.end.y}) && !isAttacked(board, BoardPosition{.x=move.start.x, .y=move.start.y});
        }
        else if (move.castle == SHORT) {
            isValid = !isAttacked(board, BoardPosition{.x=move.end.x-1, .y=move.end.y}) && !isAttacked(board, BoardPosition{.x=move.start.x, .y=move.start.y});
        }
        if (isValid) {
            makeMove(move);
            isValid = !isAttacked(board, activeColor == WHITE ? whiteKingSquare : blackKingSquare);
            undoMove();
        }
        if (isValid) {
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}


void State::makeMove(Move move) {
    const HistoricalEntry entry = {
        move,
        board[move.start.y][move.start.x],
        board[move.end.y][move.end.x],
        castlingRights,
        halfMoveClock,
        enPassantSquare
    };

    const Piece movingPiece = entry.movedPiece;
    if (std::abs(movingPiece) == WHITE_PAWN || entry.overwrittenPiece != EMPTY) {
        halfMoveClock = 0;
    } else {
        halfMoveClock++;
    }
    if (activeColor == BLACK) {
        fullMoveClock++;
    }

    const Piece newPiece = move.promotedTo.value_or(movingPiece);
    if (!sameColor(activeColor, newPiece)) {
        throw std::invalid_argument("Moving piece from wrong side.");
    }

    // Move piece
    board[move.end.y][move.end.x] = newPiece;
    board[move.start.y][move.start.x] = EMPTY;
    if (move.enPassantCapture) {
        board[move.start.y][move.end.x] = EMPTY;
    } else if (move.castle == LONG) {
        const Piece rook = board[move.start.y][0];
        board[move.start.y][0] = EMPTY;
        board[move.start.y][move.end.x + 1] = rook;
    } else if (move.castle == SHORT) {
        const Piece rook = board[move.start.y][7];
        board[move.start.y][7] = EMPTY;
        board[move.start.y][move.end.x - 1] = rook;
    }

    enPassantSquare = move.newEnPassantSquare;
    if (newPiece == WHITE_KING) {
        whiteKingSquare = move.end;
    }
    if (newPiece == BLACK_KING) {
        blackKingSquare = move.end;
    }

    // HANDLE CASTLING
    const int backRow = activeColor == WHITE ? 7 : 0;
    const bool movingKing = std::abs(newPiece) == WHITE_KING;
    const bool movingQueenSideRook = backRow == move.start.y && move.start.x == 0;
    if ((movingQueenSideRook || movingKing) && castleAllowed(activeColor, LONG, castlingRights)) {
        disAllowCastle(activeColor, LONG, castlingRights);
    }
    const bool movingKingSideRook = backRow == move.start.y && move.start.x == 7;
    if ((movingKingSideRook || movingKing) && castleAllowed(activeColor, SHORT, castlingRights)) {
        disAllowCastle(activeColor, SHORT, castlingRights);
    }

    // taking an enemy rook
    const int enemyBackRow = activeColor == WHITE ? 0 : 7;
    const Color enemyColor = activeColor == WHITE ? BLACK : WHITE;
    const bool capturingQueenSideRook = enemyBackRow == move.end.y && move.end.x == 0;
    if (capturingQueenSideRook && castleAllowed(enemyColor, LONG, castlingRights)) {
        disAllowCastle(enemyColor, LONG, castlingRights);
    }
    const bool capturingKingSideRook = enemyBackRow == move.end.y && move.end.x == 7;
    if (capturingKingSideRook && castleAllowed(enemyColor, SHORT, castlingRights)) {
        disAllowCastle(enemyColor, SHORT, castlingRights);
    }

    history.push_back(entry);
    activeColor = activeColor == WHITE ? BLACK : WHITE;
}

void State::undoMove() {
    HistoricalEntry entry = history.back();
    Move move = entry.move;
    history.pop_back();
    activeColor = activeColor == WHITE ? BLACK : WHITE;

    if (activeColor == BLACK) {
        fullMoveClock--;
    }

    const int color = activeColor == WHITE ? 1 : -1;

    if (move.enPassantCapture) {
        board[move.start.y][move.end.x] = (activeColor == WHITE) ? BLACK_PAWN : WHITE_PAWN;
    }
    else if (move.castle == LONG) {
        const Piece rook = board[move.start.y][move.end.x + 1];
        board[move.start.y][move.end.x + 1] = EMPTY;
        board[move.start.y][0] = rook;
    } else if (move.castle == SHORT) {
        const Piece rook = board[move.start.y][move.end.x - 1];
        board[move.start.y][move.end.x - 1] = EMPTY;
        board[move.start.y][7] = rook;
    }


    board[move.end.y][move.end.x] = entry.overwrittenPiece;
    board[move.start.y][move.start.x] = entry.movedPiece;
    if (entry.movedPiece == WHITE_KING) {
        whiteKingSquare = move.start;
    }
    if (entry.movedPiece == BLACK_KING) {
        blackKingSquare = move.start;
    }

    castlingRights = entry.castlingBeforeMove;
    enPassantSquare = entry.enPassantBeforeMove;
    halfMoveClock = entry.halfMoveClockBeforeMove;
}

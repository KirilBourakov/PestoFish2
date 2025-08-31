//
// Created by Kiril on 2025-08-23.
//
module;
#include <stdexcept>

module Engine;
import Move;

Engine::Engine() {
    board = get_start_board();

    moveTurn = WHITE;
    castlingRights = 0b1111;
    enPassantSquare = std::nullopt;
    halfMoveClock = 0;
    fullMoveCock = 0;

    whiteKingSquare = {.x=4, .y=7};
    blackKingSquare = {.x=4, .y=0};

}

std::vector<Move> Engine::getMoves(
    BoardArray &board,
    const Color color,
    const int castleRights,
    const BoardPosition kingPosition,
    const std::optional<BoardPosition> &enPassantSquare
) {
    /// TODO: add perft testing for this (depth 6)?

    std::vector<Move> moves;

    // get all pseudo legal moves
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (sameColor(color, board[y][x])) {
                switch (std::abs(board[y][x])) {
                    case WHITE_PAWN:
                        addPawnMoves(board, x, y, color, enPassantSquare, moves);
                        break;
                    case WHITE_KNIGHT:
                        addKnightMoves(board, x, y, color, moves);
                        break;
                    case WHITE_BISHOP:
                        addSlidingMoves(board, x, y, color, false, true, moves);
                        break;
                    case WHITE_ROOK:
                        addSlidingMoves(board, x, y, color, true, false, moves);
                        break;
                    case WHITE_QUEEN:
                        addSlidingMoves(board, x, y, color, true, true, moves);
                        break;
                    case WHITE_KING:
                        addKingMoves(board, x, y, color, castleRights, moves);
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
            isValid = !isAttacked(board, BoardPosition{.x=move.end.x+1, .y=move.end.y}) && !isAttacked(board, BoardPosition{.x=move.start.x, .y=move.start.y});
        }
        else if (move.castle == SHORT) {
            isValid = !isAttacked(board, BoardPosition{.x=move.end.x-1, .y=move.end.y}) && !isAttacked(board, BoardPosition{.x=move.start.x, .y=move.start.y});
        }
        if (isValid) {
            // simulate move
            Piece moved = board[move.start.y][move.start.x];
            Piece captured = board[move.end.y][move.end.x];
            std::optional<Piece> enPassantPiece = std::nullopt;
            if (move.enPassant.has_value()) {
                BoardPosition enPassantPosition = move.enPassant.value();
                enPassantPiece = board[enPassantPosition.y][enPassantPosition.x];
            }

            BoardPosition newKingPos = kingPosition;
            if (std::abs(board[move.start.y][move.start.x]) == WHITE_KING) {
                newKingPos = move.end;
            }

            this->simulateMove(board, move);
            isValid = !isAttacked(board, newKingPos);
            this->undoSimulateMove(board, move, moved, captured, enPassantPiece);
        }
        if (isValid) {
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}
void Engine::simulateMove(BoardArray& board, const Move &move) {
    const Piece newPiece = move.promotion.value_or(board[move.start.y][move.start.x]);
    board[move.end.y][move.end.x] = newPiece;
    board[move.start.y][move.start.x] = EMPTY;
    if (move.enPassant.has_value()) {
        BoardPosition enPassantPosition = move.enPassant.value();
        board[enPassantPosition.y][enPassantPosition.x] = EMPTY;
    }

    if (move.castle == LONG) {
        const Piece rook = board[move.end.y][0];
        board[move.end.y][0] = EMPTY;
        board[move.end.y][move.end.x+1] = rook;
    }
    else if (move.castle == SHORT) {
        const Piece rook = board[move.end.y][7];
        board[move.end.y][7] = EMPTY;
        board[move.end.y][move.end.x-1] = rook;
    }
}

void Engine::undoSimulateMove(BoardArray& board, const Move &move, const Piece moved, const Piece captured, std::optional<Piece> enPassantPiece) {
    board[move.start.y][move.start.x] = moved;
    board[move.end.y][move.end.x] = captured;
    if (enPassantPiece.has_value() && move.enPassant.has_value()) {
        BoardPosition enPassantPosition = move.enPassant.value();
        board[enPassantPosition.y][enPassantPosition.x] = enPassantPiece.value();
    }
    else if ((enPassantPiece.has_value() && !move.enPassant.has_value()) || (!enPassantPiece.has_value() && move.enPassant.has_value())) {
        throw std::invalid_argument("EnPassant value mismatch!");
    }

    int color = moved < 0 ? -1 : 1;
    if (move.castle == LONG) {
        board[move.end.y][0] = static_cast<Piece>(color * WHITE_ROOK);
        board[move.end.y][move.end.x+1] = EMPTY;
    }
    else if (move.castle == SHORT) {
        board[move.end.y][7] = static_cast<Piece>(color * WHITE_ROOK);
        board[move.end.y][move.end.x-1] = EMPTY;
    }
}

int Engine::add(int a, int b) {
    return a + b;
}


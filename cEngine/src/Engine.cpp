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

std::vector<Move> Engine::getMoves(const BoardArray& board, const Color color, int castleRights, const std::optional<BoardPosition>& enPassantSquare) {
    std::vector<Move> moves;

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

    // TODO: purge illegal
    return moves;
}

void Engine::makeMove(Move move) {
    // TODO: handle castling, enpassent
    Piece newPiece = move.promotion.value_or(board[move.start.y][move.start.x]);
    board[move.end.y][move.end.x] = newPiece;
    board[move.start.y][move.start.x] = EMPTY;
}

int Engine::add(int a, int b) {
    return a + b;
}


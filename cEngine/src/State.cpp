//
// Created by Kiril on 2025-08-31.
//
#include "ModuleOnly/CastleUtils.hpp"
#include "ModuleOnly/Enums.hpp"
#include "ModuleOnly/Move.hpp"
#include <iostream>
#include <stdexcept>

#include "State.hpp"

#include <sstream>

State::State()
    : board(getStartingBoard())
    , activeColor(Color::White)
    , castlingRights(0b1111)
    , enPassantSquare(std::nullopt)
    , halfMoveClock(0)
    , fullMoveClock(1)
    , whiteKingSquare{4, 7}
    , blackKingSquare{4, 0}
    , hash(board, activeColor, castlingRights, enPassantSquare) {}

State::State(const Board& board, const Color activeColor, const int castlingRights,
             const std::optional<BoardPosition> enPassantSquare, const int halfMoveClock, const int fullMoveClock)
    : board(board)
    , activeColor(activeColor)
    , castlingRights(castlingRights)
    , enPassantSquare(enPassantSquare)
    , halfMoveClock(halfMoveClock)
    , fullMoveClock(fullMoveClock)
    , hash(board, activeColor, castlingRights, enPassantSquare) {

    // find king squares
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (this->board.at(y, x) == Pieces::BLACK_KING) {
                blackKingSquare = {x, y};
            } else if (this->board.at(y, x) == Pieces::WHITE_KING) {
                whiteKingSquare = {x, y};
            }
        }
    }
}

State::State(const Board& board, const Color activeColor, const int castlingRights,
             const std::optional<BoardPosition> enPassantSquare)
    : State(board, activeColor, castlingRights, enPassantSquare, activeColor == Color::White ? 0 : 1, 1) {}

State::State(const Board& board, const Color activeColor, const int castlingRights,
             const std::optional<BoardPosition> enPassantSquare, const int halfMoveClock, const int fullMoveClock,
             const BoardPosition whiteKingSquare, const BoardPosition blackKingSquare, const std::vector<HistoricalEntry>& history,
             const std::vector<u64>& hashHistory)
    : board(board)
    , activeColor(activeColor)
    , castlingRights(castlingRights)
    , enPassantSquare(enPassantSquare)
    , halfMoveClock(halfMoveClock)
    , fullMoveClock(fullMoveClock)
    , whiteKingSquare(whiteKingSquare)
    , blackKingSquare(blackKingSquare)
    , history(history)
    , hashHistory(hashHistory)
    , hash(board, activeColor, castlingRights, enPassantSquare) {}

std::vector<Move> State::getMoves() {
    return purgeIllegal(board.getPseudoLegal(activeColor, enPassantSquare, castlingRights));
}

void State::translateAndMove(BoardPosition start, BoardPosition end, std::optional<Pieces::Piece> promotedTo) {
    std::vector<Move> moves;
    board.addMoves(start.x, start.y, activeColor, enPassantSquare, castlingRights, moves);
    moves = purgeIllegal(moves);
    for (auto move : moves) {
        if (move.start == start && move.end == end && move.promotedTo == promotedTo) {
            makeMove(move);
            return;
        }
    }
    throw std::invalid_argument("Illegal move");
}

GameState State::getGameState(const std::vector<Move>& possibleMoves) const {
    if (isHalfMoveTie()) {
        return GameState::DRAW;
    }

    int count = 1;
    for (int i = hashHistory.size() - 1; i >= 0; i--) {
        if (hashHistory.at(i) == hash.getValue()) {
            count++;
            if (count == 3) {
                return GameState::DRAW;
            }
        }
    }

    if (possibleMoves.empty()) {
        // is the current color in check, other color wins
        if (colorInCheck(activeColor)) {
            if (activeColor == Color::White) {
                return GameState::BLACK_WIN;
            }
            return GameState::WHITE_WIN;
        }
        // otherwise, stalemate
        return GameState::STALEMATE;
    }

    if (board.isDrawFromMaterial()) {
        return GameState::DRAW;
    }

    return GameState::IN_PLAY;
}

GameState State::getGameState() {
    return getGameState(getMoves());
}

State State::makeThreadCopy() const {
    Board copyBoard = board;                 // TODO: make sure this is a copy
    std::vector<HistoricalEntry> historyCopy{}; // threads don't need game history
    std::vector<u64> hashCopy = hashHistory;
    return {copyBoard,     activeColor,     castlingRights,  enPassantSquare, halfMoveClock,
            fullMoveClock, whiteKingSquare, blackKingSquare, historyCopy,     hashCopy};
}

bool State::isLegalMove(BoardPosition start, BoardPosition end) {
    std::vector<Move> moves;
    board.addMoves(start.x, start.y, activeColor, enPassantSquare, castlingRights, moves);
    moves = purgeIllegal(moves);
    for (auto move : moves) {
        if (move.start == start && move.end == end) {
            return true;
        }
    }
    return false;
}

std::vector<Move> State::purgeIllegal(const std::vector<Move>& pseudolegalMoves) {
    std::vector<Move> legalMoves;
    for (const Move move : pseudolegalMoves) {
        bool isValid = true;
        const Color color = Pieces::piece_color(board.at(move.start.y, move.start.x));
        if (move.castle == CastleType::LONG) {
            isValid = !board.isAttacked(BoardPosition{.x = move.end.x + 1, .y = move.end.y}, color) &&
                      !board.isAttacked(BoardPosition{.x = move.end.x + 2, .y = move.end.y}, color) &&
                      !board.isAttacked(BoardPosition{.x = move.start.x, .y = move.start.y}, color);
        } else if (move.castle == CastleType::SHORT) {
            isValid = !board.isAttacked(BoardPosition{.x = move.end.x - 1, .y = move.end.y}, color) &&
                      !board.isAttacked(BoardPosition{.x = move.start.x, .y = move.start.y}, color);
        }
        if (isValid) {
            const Color preMoveColor = activeColor;
            makeMove(move);
            isValid = !board.isAttacked(preMoveColor == Color::White ? whiteKingSquare : blackKingSquare);
            undoMove();
        }
        if (isValid) {
            legalMoves.push_back(move);
        }
    }
    return legalMoves;
}

// Updated peicewise within State
void State::makeMove(const Move& move) {
    const u64 preMoveHash = hash.getValue();

    const HistoricalEntry entry = {
        move,
        board.at(move.start.y, move.start.x),
        board.at(move.end.y, move.end.x),
        castlingRights,
        halfMoveClock,
        enPassantSquare,
    };
    const Pieces::Piece movingPiece = entry.movedPiece;
    const Pieces::Piece newPiece = move.promotedTo.value_or(entry.movedPiece);


    // Error checking
    if (!Pieces::sameColor(activeColor, newPiece)) {
        std::cout << "Illegal move: " << move.start << "->" << move.end << " moving " << newPiece << std::endl;
        throw std::invalid_argument("Moving piece from wrong side.");
    }

    // UPDATE STATE INFO
    if (bool pawnMoveOrAttack = Pieces::piece_type(movingPiece) == PieceType::Pawn || entry.overwrittenPiece != Pieces::EMPTY) {
        halfMoveClock = 0;
    } else {
        halfMoveClock++;
    }
    if (activeColor == Color::Black) {
        fullMoveClock++;
    }

    hash.makeMove(move, entry.movedPiece, entry.overwrittenPiece);

    if (enPassantSquare != move.newEnPassantSquare) {
        hash.changeEnPassantSquare(enPassantSquare, move.newEnPassantSquare);
    }
    enPassantSquare = move.newEnPassantSquare;

    // UPDATE KING POS
    if (newPiece == Pieces::WHITE_KING) {
        whiteKingSquare = move.end;
    }
    if (newPiece == Pieces::BLACK_KING) {
        blackKingSquare = move.end;
    }

    updateCastlingRights(move, newPiece);

    // MOVE PIECE
    board.move(move, entry.movedPiece, entry.overwrittenPiece);

    activeColor = activeColor == Color::White ? Color::Black : Color::White;
    hash.flipActiveColor();

    history.push_back(entry);
    hashHistory.push_back(preMoveHash);
}

void State::updateCastlingRights(const Move& move, const Pieces::Piece newPiece) {
    const int backRow = activeColor == Color::White ? 7 : 0;
    const bool movingKing = Pieces::piece_type(newPiece) == PieceType::King;
    const bool movingQueenSideRook = backRow == move.start.y && move.start.x == 0;
    int oldRights = castlingRights;
    bool updated = false;
    if ((movingQueenSideRook || movingKing) && castleAllowed(activeColor, CastleType::LONG, castlingRights)) {
        disAllowCastle(activeColor, CastleType::LONG, castlingRights);
        updated = true;
    }
    const bool movingKingSideRook = backRow == move.start.y && move.start.x == 7;
    if ((movingKingSideRook || movingKing) && castleAllowed(activeColor, CastleType::SHORT, castlingRights)) {
        disAllowCastle(activeColor, CastleType::SHORT, castlingRights);
        updated = true;
    }

    // taking an enemy rook
    const int enemyBackRow = activeColor == Color::White ? 0 : 7;
    const Color enemyColor = activeColor == Color::White ? Color::Black : Color::White;
    const bool capturingQueenSideRook = enemyBackRow == move.end.y && move.end.x == 0;
    if (capturingQueenSideRook && castleAllowed(enemyColor, CastleType::LONG, castlingRights)) {
        disAllowCastle(enemyColor, CastleType::LONG, castlingRights);
        updated = true;
    }
    const bool capturingKingSideRook = enemyBackRow == move.end.y && move.end.x == 7;
    if (capturingKingSideRook && castleAllowed(enemyColor, CastleType::SHORT, castlingRights)) {
        disAllowCastle(enemyColor, CastleType::SHORT, castlingRights);
        updated = true;
    }
    if (updated) {
        hash.changeCastling(oldRights, castlingRights);
    }
}


void State::undoMove() {
    auto [move, movedPiece, overwrittenPiece, castlingBeforeMove, halfMoveClockBeforeMove, enPassantBeforeMove] = history.back();
    const u64 historicalHash = hashHistory.back();
    history.pop_back();
    hashHistory.pop_back();

    activeColor = activeColor == Color::White ? Color::Black : Color::White;
    board.undoMove(move, movedPiece, overwrittenPiece, activeColor);

    if (activeColor == Color::Black) {
        fullMoveClock--;
    }
    if (movedPiece == Pieces::WHITE_KING) {
        whiteKingSquare = move.start;
    }
    else if (movedPiece == Pieces::BLACK_KING) {
        blackKingSquare = move.start;
    }

    castlingRights = castlingBeforeMove;
    enPassantSquare = enPassantBeforeMove;
    halfMoveClock = halfMoveClockBeforeMove;
    hash.setValue(historicalHash);
}

bool State::samePosition(const State& other) const {
    return board == other.board && activeColor == other.activeColor && castlingRights == other.castlingRights &&
           enPassantSquare == other.enPassantSquare;
}

bool operator==(const State& lhs, const State& rhs) {
    return lhs.board == rhs.board && lhs.activeColor == rhs.activeColor && lhs.castlingRights == rhs.castlingRights &&
           lhs.enPassantSquare == rhs.enPassantSquare && lhs.halfMoveClock == rhs.halfMoveClock &&
           lhs.fullMoveClock == rhs.fullMoveClock && lhs.whiteKingSquare == rhs.whiteKingSquare &&
           lhs.blackKingSquare == rhs.blackKingSquare && lhs.history == rhs.history && lhs.hash == rhs.hash &&
           lhs.hashHistory == rhs.hashHistory;
}

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

State::State(const NewBoard& board, const Color activeColor, const int castlingRights,
             const std::optional<BoardPosition> enPassantSquare, const int halfMoveClock, const int fullMoveClock)
    : board(board)
    , activeColor(activeColor)
    , castlingRights(castlingRights)
    , enPassantSquare(enPassantSquare)
    , halfMoveClock(halfMoveClock)
    , fullMoveClock(fullMoveClock)
    , hash(board, activeColor, castlingRights, enPassantSquare) {

    // find king squares
    for (int y = 0; y < this->board.size; ++y) {
        for (int x = 0; x < this->board.size; ++x) {
            if (this->board(y, x) == Pieces::BLACK_KING) {
                blackKingSquare = {x, y};
            } else if (this->board(y, x) == Pieces::WHITE_KING) {
                whiteKingSquare = {x, y};
            }
        }
    }
}

State::State(const NewBoard& board, const Color activeColor, const int castlingRights,
             const std::optional<BoardPosition> enPassantSquare)
    : State(board, activeColor, castlingRights, enPassantSquare, activeColor == Color::White ? 0 : 1, 1) {}

State::State(const NewBoard& board, const Color activeColor, const int castlingRights,
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

State State::fromFen(const std::string& fen) {
    std::istringstream iss(fen);
    const std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    if (tokens.size() != 6) {
        throw std::invalid_argument("Invalid FEN: must contain 6 fields");
    }

    const std::string& board_rep = tokens[0];
    std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> board{};
    int x = 0;
    int y = 0; // 0,0 is top left of board
    for (const char ch : board_rep) {
        if (x > BOARD_SIZE || y >= BOARD_SIZE) {
            throw std::invalid_argument("Invalid FEN: must contain board representation within BOARD_SIZE");
        }
        if (ch == '/') {
            if (x != 8) {
                throw std::invalid_argument("Invalid FEN: rank has incorrect number of squares");
            }
            y++;
            x = 0;
        } else if (isalpha(ch)) {
            board[y][x] = Pieces::from_char(ch);
            x++;
        } else {
            int skips = ch - '0';
            if (skips < 1 || skips > 8) {
                throw std::invalid_argument("Invalid FEN: skip digit must be 1–8");
            }

            for (int i = 0; i < skips; ++i) {
                board[y][x] = Pieces::EMPTY;
                x++;
            }
        }
    }

    Color activeColor;
    if (tokens[1] == "w") {
        activeColor = Color::White;
    } else if (tokens[1] == "b") {
        activeColor = Color::Black;
    } else {
        throw std::invalid_argument("Invalid FEN: active color must be 'w' or 'b'");
    }

    int castleRights = 0;
    if (tokens[2] != "-") {
        for (const char c : tokens[2]) {
            char lowered = tolower(c);
            Color color = lowered == c ? Color::Black : Color::White;
            switch (lowered) {
            case 'k':
                allowCastle(color, CastleType::SHORT, castleRights);
                break;
            case 'q':
                allowCastle(color, CastleType::LONG, castleRights);
                break;
            default:
                throw std::invalid_argument("Invalid castle type");
            }
        }
    }

    std::optional<BoardPosition> enPassantSquare = std::nullopt;
    if (tokens[3] != "-") {
        enPassantSquare = BoardPosition::fromSquare(tokens[3]);
    }

    int halfMoveClock = std::stoi(tokens[4]);
    int fullMoveClock = std::stoi(tokens[5]);

    return {NewBoard(board), activeColor, castleRights, enPassantSquare, halfMoveClock, fullMoveClock};
}

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

GameState State::getGameState(const std::vector<Move>& possibleMoves) {
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

    return GameState::IN_PLAY;
}

GameState State::getGameState() {
    return getGameState(getMoves());
}

State State::makeThreadCopy() {
    NewBoard copyBoard = board;                 // TODO: make sure this is a copy
    std::vector<HistoricalEntry> historyCopy{}; // threads don't need game history
    std::vector<u64> hashCopy = hashHistory;
    return {copyBoard,     activeColor,     castlingRights,  enPassantSquare, halfMoveClock,
            fullMoveClock, whiteKingSquare, blackKingSquare, historyCopy,     hashCopy};
}

bool State::isLegalMove(BoardPosition start, BoardPosition end) {
    std::vector<Move> moves;
    board.addMoves(start.x, start.y, activeColor, enPassantSquare, castlingRights, moves);
    moves = purgeIllegal(moves);
    std::cout << "MOVE: " << start << " -> " << end << std::endl;
    for (auto move : moves) {
        std::cout << move << std::endl;
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
        const Color color = Pieces::piece_color(board(move.start.y, move.start.x));
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
    const HistoricalEntry entry = {
        move, board(move.start.y, move.start.x), board(move.end.y, move.end.x), castlingRights, halfMoveClock, enPassantSquare,
    };
    const u64 preMoveHash = hash.getValue();

    const Pieces::Piece movingPiece = entry.movedPiece;
    if (Pieces::piece_type(movingPiece) == PieceType::Pawn || entry.overwrittenPiece != Pieces::EMPTY) {
        halfMoveClock = 0;
    } else {
        halfMoveClock++;
    }
    if (activeColor == Color::Black) {
        fullMoveClock++;
    }

    const Pieces::Piece newPiece = move.promotedTo.value_or(movingPiece);
    if (!Pieces::sameColor(activeColor, newPiece)) {
        std::cout << "Illegal move: " << move.start << "->" << move.end << " moving " << newPiece << std::endl;
        throw std::invalid_argument("Moving piece from wrong side.");
    }

    // MOVE PIECE
    hash.makeMove(move, board(move.start.y, move.start.x), board(move.end.y, move.end.x));
    board(move.end.y, move.end.x) = newPiece;
    board(move.start.y, move.start.x) = Pieces::EMPTY;
    if (move.enPassantCapture) {
        board(move.start.y, move.end.x) = Pieces::EMPTY;
    } else if (move.castle == CastleType::LONG) {
        const Pieces::Piece rook = board(move.start.y, 0);
        board(move.start.y, 0) = Pieces::EMPTY;
        board(move.start.y, move.end.x + 1) = rook;
    } else if (move.castle == CastleType::SHORT) {
        const Pieces::Piece rook = board(move.start.y, 7);
        board(move.start.y, 7) = Pieces::EMPTY;
        board(move.start.y, move.end.x - 1) = rook;
    }

    // UPDATE EN PASSENT
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

    // HANDLE CASTLING
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

    activeColor = activeColor == Color::White ? Color::Black : Color::White;
    hash.flipActiveColor();

    history.push_back(entry);
    hashHistory.push_back(preMoveHash);
}

void State::undoMove() {
    using namespace Pieces;

    HistoricalEntry entry = history.back();
    u64 historicalHash = hashHistory.back();
    history.pop_back();
    hashHistory.pop_back();

    Move move = entry.move;
    activeColor = activeColor == Color::White ? Color::Black : Color::White;

    if (activeColor == Color::Black) {
        fullMoveClock--;
    }

    const int color = activeColor == Color::White ? 1 : -1;

    if (move.enPassantCapture) {
        board(move.start.y, move.end.x) = (activeColor == Color::White) ? BLACK_PAWN : WHITE_PAWN;
    } else if (move.castle == CastleType::LONG) {
        const Piece rook = board(move.start.y, move.end.x + 1);
        board(move.start.y, move.end.x + 1) = EMPTY;
        board(move.start.y, 0) = rook;
    } else if (move.castle == CastleType::SHORT) {
        const Piece rook = board(move.start.y, move.end.x - 1);
        board(move.start.y, move.end.x - 1) = EMPTY;
        board(move.start.y, 7) = rook;
    }

    board(move.end.y, move.end.x) = entry.overwrittenPiece;
    board(move.start.y, move.start.x) = entry.movedPiece;
    if (entry.movedPiece == WHITE_KING) {
        whiteKingSquare = move.start;
    }
    if (entry.movedPiece == BLACK_KING) {
        blackKingSquare = move.start;
    }

    castlingRights = entry.castlingBeforeMove;
    enPassantSquare = entry.enPassantBeforeMove;
    halfMoveClock = entry.halfMoveClockBeforeMove;
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

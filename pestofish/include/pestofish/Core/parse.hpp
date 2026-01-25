//
// Created by Kiril on 2025-11-15.
//

#pragma once
#include <iterator>
#include <sstream>
#include <string>

#include "pestofish/Core/State.hpp"

inline BoardPosition squareToPos(const std::string& square);
inline std::string posToSquare(const BoardPosition pos);

inline std::string longAlgebricFromMove(const Move& move) {
    std::string mv = posToSquare(move.getStart()) + posToSquare(move.getEnd());
    if (move.getPromotedTo().has_value()) {
        char ch;
        switch (Pieces::piece_type(move.getPromotedTo().value())) {
        case PieceType::Queen:
            ch = 'q';
            break;
        case PieceType::Rook:
            ch = 'r';
            break;
        case PieceType::Knight:
            ch = 'n';
            break;
        case PieceType::Bishop:
            ch = 'b';
            break;
        }
        mv.push_back(ch);
    }
    return mv;
}

/**
 * Converts a string of long algebraic notation,
 * @param mv Move in long algebraic notation (e2e4, e7e8q, etc.)
 * @param state current board state
 * @return Move
 */
inline Move moveFromLongAlgebric(const std::string& mv, const State& state) {
    if (mv.size() != 4 && mv.size() != 5) {
        std::cout << "Invalid move string: " << mv << std::endl;
        throw std::invalid_argument("Invalid move string");
    }

    const BoardPosition start = squareToPos(mv.substr(0, 2));
    const BoardPosition end = squareToPos(mv.substr(2, 2));

    const Pieces::Piece moved = state.getAt(start);
    const Color movedColor = Pieces::piece_color(moved);
    const PieceType movedType = Pieces::piece_type(moved);

    // Promotion is given
    uint16_t promo = 0;
    if (mv.size() == 5) {
        if (start.x != end.x) {
            switch (mv[4]) {
                case 'q':
                    promo = Moves::PROMO_Q_CAP;
                    break;
                case 'r':
                    promo = Moves::PROMO_R_CAP;
                    break;
                case 'b':
                    promo = Moves::PROMO_B_CAP;
                    break;
                case 'n':
                    promo = Moves::PROMO_N_CAP;
                    break;
                default:
                    throw std::invalid_argument("Invalid promotion");
            }
        } else {
            switch (mv[4]) {
                case 'q':
                    promo = Moves::PROMO_Q;
                    break;
                case 'r':
                    promo = Moves::PROMO_R;
                    break;
                case 'b':
                    promo = Moves::PROMO_B;
                    break;
                case 'n':
                    promo = Moves::PROMO_N;
                    break;
                default:
                    throw std::invalid_argument("Invalid promotion");
            }
        }
        return Move::promotionMove(start, end, promo);
    }

    // Moving king (maybe castling)
    if (movedType == PieceType::King) {
        int fileDelta = end.x - start.x;
        if (fileDelta == 2) {
            return Move::castleMove<CastleType::SHORT>(start, end);
        }
        if (fileDelta == -2) {
            return Move::castleMove<CastleType::LONG>(start, end);
        }
    }

    // Moving Pawn (maybe enpassent)
    if (movedType == PieceType::Pawn) {
        // we change X and end is emtpy = enpassent
        if (start.x != end.x && state.getAt(end) == Pieces::EMPTY && state.getEnPassantSquare().has_value() &&
            end == state.getEnPassantSquare().value()) {
            return Move::enPassantCaptureMove(start, end);
        }
        if (abs(start.y - end.y) == 2) {
            return Move::doublePawnMove(start, end);
        }
    }

    return Move::standardMove(start, end);
}

inline std::string posToSquare(const BoardPosition pos) {
    char file = 'a' + pos.x;
    char rank = '8' - pos.y; // inverse of (8 - rank)
    return std::string() + file + rank;
}

inline BoardPosition squareToPos(const std::string& square) {
    const int file = square[0] - 'a';
    const int rank = square[1] - '0';

    return {file, 8 - rank}; // a8 is 0,0
}

inline State fenToState(const std::string& fen) {
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
        enPassantSquare = squareToPos(tokens[3]);
    }

    int halfMoveClock = std::stoi(tokens[4]);
    int fullMoveClock = std::stoi(tokens[5]);

    return {Board(board), activeColor, castleRights, enPassantSquare, halfMoveClock, fullMoveClock};
}

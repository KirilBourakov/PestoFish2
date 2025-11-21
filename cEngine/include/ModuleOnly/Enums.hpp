//
// Created by Kiril on 2025-10-26.
//
#pragma once
#include <cctype>
#include <cstdint>
#include <stdexcept>

enum class CastleType { SHORT = 0, LONG = 1 };

enum class GameState { IN_PLAY = 0, WHITE_WIN = 1, BLACK_WIN = -1, STALEMATE = 2, DRAW = 3 };

enum class Color : uint8_t {
    White = 0,
    Black = 1,
};

enum class PieceType : uint8_t { None = 0, Pawn, Knight, Bishop, Rook, Queen, King };

namespace Pieces {
using Piece = uint8_t;

constexpr Piece make_piece(Color c, PieceType t) noexcept {
    return (static_cast<uint8_t>(c) << 3) | static_cast<uint8_t>(t);
}

constexpr Piece EMPTY = make_piece(Color::White, PieceType::None);
constexpr Piece WHITE_PAWN = make_piece(Color::White, PieceType::Pawn);
constexpr Piece WHITE_KNIGHT = make_piece(Color::White, PieceType::Knight);
constexpr Piece WHITE_BISHOP = make_piece(Color::White, PieceType::Bishop);
constexpr Piece WHITE_ROOK = make_piece(Color::White, PieceType::Rook);
constexpr Piece WHITE_QUEEN = make_piece(Color::White, PieceType::Queen);
constexpr Piece WHITE_KING = make_piece(Color::White, PieceType::King);

constexpr Piece BLACK_PAWN = make_piece(Color::Black, PieceType::Pawn);
constexpr Piece BLACK_KNIGHT = make_piece(Color::Black, PieceType::Knight);
constexpr Piece BLACK_BISHOP = make_piece(Color::Black, PieceType::Bishop);
constexpr Piece BLACK_ROOK = make_piece(Color::Black, PieceType::Rook);
constexpr Piece BLACK_QUEEN = make_piece(Color::Black, PieceType::Queen);
constexpr Piece BLACK_KING = make_piece(Color::Black, PieceType::King);

constexpr Color piece_color(const Piece p) {
    if (p == EMPTY) {
        throw std::invalid_argument("Getting Color of empty square");
    }
    const auto color = static_cast<Color>(p >> 3);
    return color;
}

constexpr PieceType piece_type(const Piece p) noexcept {
    return static_cast<PieceType>(p & 0b111);
}

inline Piece from_char(const char ch) {
    const char lowered = tolower(ch);

    PieceType piece_type;
    switch (lowered) {
    case 'p':
        piece_type = PieceType::Pawn;
        break;
    case 'n':
        piece_type = PieceType::Knight;
        break;
    case 'b':
        piece_type = PieceType::Bishop;
        break;
    case 'r':
        piece_type = PieceType::Rook;
        break;
    case 'q':
        piece_type = PieceType::Queen;
        break;
    case 'k':
        piece_type = PieceType::King;
        break;
    default:
        throw std::invalid_argument("Invalid piece type");
    }
    return make_piece(lowered == ch ? Color::Black : Color::White, piece_type);
}

inline char piece_char(const Piece& p) {
    if (piece_type(p) == PieceType::None)
        return '.';

    char c;
    switch (piece_type(p)) {
    case PieceType::Pawn:
        c = 'P';
        break;
    case PieceType::Knight:
        c = 'N';
        break;
    case PieceType::Bishop:
        c = 'B';
        break;
    case PieceType::Rook:
        c = 'R';
        break;
    case PieceType::Queen:
        c = 'Q';
        break;
    case PieceType::King:
        c = 'K';
        break;
    default:
        c = '?';
        break;
    }

    // Lowercase for black pieces
    return (piece_color(p) == Color::Black) ? std::tolower(c) : c;
}

inline bool sameColor(const Color color, const Pieces::Piece piece) {
    if (piece == Pieces::EMPTY) {
        return false;
    }
    return Pieces::piece_color(piece) == color;
}
} // namespace Pieces

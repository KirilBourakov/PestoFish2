//
// Created by Kiril on 2025-08-27.
//
#pragma once
#include <iostream>
#include <optional>
#include <thread>

#include "Enums.hpp"
#include "Utils.hpp"

struct BoardPosition {
    int x, y;

    constexpr bool operator==(const BoardPosition& other) const {
        return x == other.x && y == other.y;
    }

    constexpr int asInt() const {
        return y * BOARD_SIZE + x;
    }

    static constexpr BoardPosition fromInt(const int in) {
        return {in & 7, in >> 3};
    }
};
inline std::ostream& operator<<(std::ostream& os, const BoardPosition& pos) {
    return os << static_cast<char>(pos.x + 97) << (abs(pos.y - 8)) << "(" << pos.x << "," << pos.y << ")";
}

namespace Moves {
    const uint16_t SOURCE_MASK = 0x3F;       // 0000 0000 0011 1111
    const uint16_t TARGET_MASK = 0xFC0;      // 0000 1111 1100 0000
    const uint16_t FLAG_MASK = 0xF000;     // 1111 0000 0000 0000

    const uint16_t QUIET = 0;
    const uint16_t DOUBLE_PUSH = 1 << 12;
    const uint16_t SHORT_CASTLE = 2 << 12;
    const uint16_t LONG_CASTLE = 3 << 12;
    const uint16_t CAPTURE = 4 << 12;
    const uint16_t EP_CAPTURE = 5 << 12;
    const uint16_t PROMO_N = 8 << 12;
    const uint16_t PROMO_B = 9 << 12;
    const uint16_t PROMO_R = 10 << 12;
    const uint16_t PROMO_Q = 11 << 12;
    const uint16_t PROMO_N_CAP = 12 << 12;
    const uint16_t PROMO_B_CAP = 13 << 12;
    const uint16_t PROMO_R_CAP = 14 << 12;
    const uint16_t PROMO_Q_CAP = 15 << 12;
}

struct Move {
    constexpr Move() = default;

    [[nodiscard]] constexpr std::optional<BoardPosition> getNewEnPassantSquare() const {
        if ((moveEncoding & Moves::FLAG_MASK) == Moves::DOUBLE_PUSH) {
            BoardPosition start = getStart();
            BoardPosition end = getEnd();
            return std::make_optional(BoardPosition{start.x, (start.y + end.y) / 2});
        }
        return std::nullopt;
    }

    [[nodiscard]] constexpr std::optional<CastleType> getCastle() const {
        const uint16_t flag = moveEncoding & Moves::FLAG_MASK;
        if (flag == Moves::SHORT_CASTLE) {return CastleType::SHORT;}
        if (flag == Moves::LONG_CASTLE) {return CastleType::LONG;}
        return std::nullopt;
    }

    [[nodiscard]] constexpr std::optional<Pieces::Piece> getPromotedTo() const {
        const uint16_t flag = moveEncoding & Moves::FLAG_MASK;

        // Check if promotion bit
        if (flag & (8 << 12)) {
             // Extract type: N=0, B=1, R=2, Q=3 (relative to base)
             // We want: Knight(2), Bishop(3), Rook(4), Queen(5)
             const auto type = static_cast<PieceType>(((flag >> 12) & 3) + 2);
             const Color color = to() < 8 ? Color::White : Color::Black;
             return Pieces::make_piece(color, type);
        }
        return std::nullopt;
    }

    [[nodiscard]] constexpr bool isCapture() const {
        return moveEncoding & Moves::FLAG_MASK;
    }
    
    [[nodiscard]] constexpr bool getEnPassantCapture() const {
        return (moveEncoding & Moves::FLAG_MASK) == Moves::EP_CAPTURE;
    }

    [[nodiscard]] constexpr BoardPosition getEnd() const {
        return BoardPosition::fromInt(to());
    }

    [[nodiscard]] constexpr BoardPosition getStart() const {
        return BoardPosition::fromInt(from());
    }
    
    [[nodiscard]] constexpr int from() const {
        return moveEncoding & Moves::SOURCE_MASK;
    }

    [[nodiscard]] constexpr int to() const {
        return (moveEncoding & Moves::TARGET_MASK) >> 6;
    }
    
    [[nodiscard]] constexpr uint16_t raw() const {
        return moveEncoding;
    }

    static constexpr Move standardMove(BoardPosition start, BoardPosition end) {
        return {static_cast<uint16_t>(start.asInt() | end.asInt() << 6)};
    }
    
    static constexpr Move promotionMove(BoardPosition start, BoardPosition end, uint16_t promotionFlag) {
        return {static_cast<uint16_t>(start.asInt() | end.asInt() << 6 | promotionFlag)};
    }
    
    template<CastleType type>
    static constexpr Move castleMove(BoardPosition start, BoardPosition end) {
        uint16_t flag = (type == CastleType::SHORT) ? Moves::SHORT_CASTLE : Moves::LONG_CASTLE;
        return {static_cast<uint16_t>(start.asInt() | end.asInt() << 6 | flag)};
    }
    
    static constexpr Move doublePawnMove(BoardPosition start, BoardPosition end) {
        return {static_cast<uint16_t>(start.asInt() | end.asInt() << 6 | Moves::DOUBLE_PUSH)};
    }
    
    static constexpr Move enPassantCaptureMove(BoardPosition start, BoardPosition end) {
        return {static_cast<uint16_t>(start.asInt() | end.asInt() << 6 | Moves::EP_CAPTURE)};
    }
    
    static constexpr Move invalid() {
        return {0};
    }

    constexpr bool operator==(const Move& other) const {
        return other.moveEncoding == moveEncoding;
    }

private:
    uint16_t moveEncoding;
    constexpr Move(const uint16_t val) : moveEncoding(val) {};
};
inline std::ostream& operator<<(std::ostream& os, const Move& m) {
    return os << m.getStart() << " -> " << m.getEnd();
}

using OptionalMove = std::optional<Move>;

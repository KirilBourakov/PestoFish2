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

    bool operator==(const BoardPosition& other) const {
        return x == other.x && y == other.y;
    }

    int asInt() const {
        return y * BOARD_SIZE + x;
    }

    static BoardPosition fromInt(const int in) {
        return {in % BOARD_SIZE, in / BOARD_SIZE};
    }
};
inline std::ostream& operator<<(std::ostream& os, const BoardPosition& pos) {
    return os << static_cast<char>(pos.x + 97) << (abs(pos.y - 8)) << "(" << pos.x << "," << pos.y << ")";
}

namespace Moves {
    const uint16_t SOURCE_MASK = 0x3F;       // 0000 0000 0011 1111
    const uint16_t TARGET_MASK = 0xFC0;      // 0000 1111 1100 0000
    const uint16_t FLAG_MASK   = 0xF000;     // 1111 0000 0000 0000

    const uint16_t QUIET          = 0;
    const uint16_t DOUBLE_PUSH    = 1 << 12;
    const uint16_t SHORT_CASTLE   = 2 << 12;
    const uint16_t LONG_CASTLE    = 3 << 12;
    const uint16_t CAPTURE        = 4 << 12;
    const uint16_t EP_CAPTURE     = 5 << 12;
    const uint16_t PROMO_N        = 8 << 12;
    const uint16_t PROMO_B        = 9 << 12;
    const uint16_t PROMO_R        = 10 << 12;
    const uint16_t PROMO_Q        = 11 << 12;
    const uint16_t PROMO_N_CAP    = 12 << 12;
    const uint16_t PROMO_B_CAP    = 13 << 12;
    const uint16_t PROMO_R_CAP    = 14 << 12;
    const uint16_t PROMO_Q_CAP    = 15 << 12;
}



struct Move {
    Move() = default;

    [[nodiscard]] std::optional<BoardPosition> getNewEnPassantSquare() const {
        if ((moveEncoding & Moves::FLAG_MASK) == Moves::DOUBLE_PUSH) {
            BoardPosition start = getStart();
            BoardPosition end = getEnd();
            return std::make_optional(BoardPosition{start.x, (start.y+end.y) / 2});
        }
        return std::nullopt;

    }
    [[nodiscard]] std::optional<CastleType> getCastle() const {
        uint16_t flag = moveEncoding & Moves::FLAG_MASK;
        switch (flag) {
            case Moves::SHORT_CASTLE:
                return CastleType::SHORT;
            case Moves::LONG_CASTLE:
                return CastleType::LONG;
            default:
                return std::nullopt;
        }
    }
    [[nodiscard]] std::optional<Pieces::Piece> getPromotedTo() const {
        uint16_t flag = moveEncoding & Moves::FLAG_MASK;

        PieceType type;
        switch (flag) {
            case Moves::PROMO_N:
            case Moves::PROMO_N_CAP:
                type = PieceType::Knight;
                break;

            case Moves::PROMO_B:
            case Moves::PROMO_B_CAP:
                type = PieceType::Bishop;
                break;

            case Moves::PROMO_R:
            case Moves::PROMO_R_CAP:
                type = PieceType::Rook;
                break;

            case Moves::PROMO_Q:
            case Moves::PROMO_Q_CAP:
                type = PieceType::Queen;
                break;

            default:
                return std::nullopt;
        }
        Color color = getEnd().y == 0 ? Color::White : Color::Black;
        return Pieces::make_piece(color, type);
    }
    [[nodiscard]] bool getEnPassantCapture() const {
        return (moveEncoding & Moves::FLAG_MASK) == Moves::EP_CAPTURE;
    }
    [[nodiscard]] BoardPosition getEnd() const {
        return BoardPosition::fromInt((moveEncoding & Moves::TARGET_MASK) >> 6);
    }
    [[nodiscard]] BoardPosition getStart() const {
        return BoardPosition::fromInt(moveEncoding & Moves::SOURCE_MASK);
    }

    static Move standardMove(BoardPosition start, BoardPosition end) {
        uint16_t mv = start.asInt() | end.asInt() << 6;
        return {mv};
    }
    static Move promotionMove(BoardPosition start, BoardPosition end, uint16_t promotionFlag) {
        uint16_t mv = start.asInt() | end.asInt() << 6 | promotionFlag;
        return {mv};
    }
    template<CastleType type>
    static Move castleMove(BoardPosition start, BoardPosition end) {
        uint16_t flag = type == CastleType::SHORT ? Moves::SHORT_CASTLE : Moves::LONG_CASTLE;
        uint16_t mv = start.asInt() | end.asInt() << 6 | flag;
        return {mv};
    }
    static Move doublePawnMove(BoardPosition start, BoardPosition end) {
        // int dir = movedColor == Color::White ? -1 : 1;
        // {end.x, end.y - dir}
        return {start.asInt() | end.asInt() << 6 | Moves::DOUBLE_PUSH};
    }
    static Move enPassantCaptureMove(BoardPosition start, BoardPosition end) {
        return {start.asInt() | end.asInt() << 6 | Moves::EP_CAPTURE};
    }
    static Move invalid() {
        return {0};
    }

    bool operator==(const Move& other) const {
        return other.moveEncoding == moveEncoding;
    }

private:
    uint16_t moveEncoding;
    Move(const int val) : moveEncoding(val) {};
};
inline std::ostream& operator<<(std::ostream& os, const Move& m) {
    return os << m.getStart() << " -> " << m.getEnd();
}

using OptionalMove = std::optional<Move>;

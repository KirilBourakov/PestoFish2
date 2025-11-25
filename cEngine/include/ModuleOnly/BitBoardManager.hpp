//
// Created by Kiril on 11/23/2025.
//

#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <vector>
#include <ModuleOnly/Move.hpp>

#include "Enums.hpp"

constexpr int BOARD_SIZE = 8;
class BitBoardManager {
public:
    [[nodiscard]] uint64_t at(const Pieces::Piece piece) const {
        assert(piece != Pieces::EMPTY && "EMPTY has no bitboard");
        return board[indexOf(piece)];
    }

    [[nodiscard]] uint64_t at(const Color color) const {
        return colorBoard[static_cast<size_t>(color)]; // WHITE = 0, BLACK = 1
    }

    void insert(const Pieces::Piece piece, const int y, const int x) {
        assert(piece != Pieces::EMPTY && "EMPTY has no bitboard");

        const uint64_t mask = (1ULL << shiftValue(y, x));
        at_mut(piece) |= mask;
        at_mut(Pieces::piece_color(piece)) |= mask;
    }

    /**
     * Update the bitboard with a certain move.
     * @param mv Move played
     * @param startContent the contents of the mv.start when the move was played
     * @param endContent the contents of mv.end when the move was played
     */
    void move(const Move& mv, const Pieces::Piece startContent, const Pieces::Piece endContent) {
        // remove piece
        if (endContent != Pieces::EMPTY) {
            remove(endContent, mv.end.y, mv.end.x);
        }

        // move
        remove(startContent, mv.start.y, mv.start.x);
        add(mv.promotedTo.value_or(startContent), mv.end.y, mv.end.x);

        // remove pawn behind, if en passant
        if (mv.enPassantCapture) {
            const Pieces::Piece captured = startContent == Pieces::WHITE_PAWN ? Pieces::BLACK_PAWN : Pieces::WHITE_PAWN;
            remove(captured, mv.start.y, mv.end.x);
        }

        // move rook when castling
        else if (mv.castle == CastleType::LONG) {
            const Pieces::Piece rook = startContent == Pieces::WHITE_KING ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
            remove(rook, mv.start.y, 0);
            add(rook, mv.start.y, mv.end.x+1);
        }

        else if (mv.castle == CastleType::SHORT) {
            const Pieces::Piece rook = startContent == Pieces::WHITE_KING ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
            remove(rook, mv.start.y, 7);
            add(rook, mv.start.y, mv.end.x-1);
        }
    }

    void undoMove(const Move& mv, const Pieces::Piece movedPiece, const Pieces::Piece overwrittenPiece, const Color activeColor) {
        if (mv.enPassantCapture) {
            add((activeColor == Color::White) ? Pieces::BLACK_PAWN : Pieces::WHITE_PAWN, mv.start.y, mv.end.x);
        }
        else if (mv.castle == CastleType::LONG) {
            const Pieces::Piece rook = (activeColor == Color::White) ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
            remove(rook, mv.start.y, mv.end.x+1);
            add(rook, mv.start.y, 0);
        }
        else if (mv.castle == CastleType::SHORT) {
            const Pieces::Piece rook = (activeColor == Color::White) ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
            remove(rook, mv.start.y, mv.end.x-1);
            add(rook, mv.start.y, 7);
        }

        remove(mv.promotedTo.value_or(movedPiece), mv.end.y, mv.end.x);
        add(movedPiece, mv.start.y, mv.start.x);

        if (overwrittenPiece != Pieces::EMPTY) {
            add(overwrittenPiece, mv.end.y, mv.end.x);
        }
    }

    template<Color color>
    void addPawnMoves(const std::optional<BoardPosition> enPassantSquare, std::vector<Move>& moves) const {
        using namespace Pieces;
        static constexpr std::array<Piece, 4> whitePieces = {WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN};
        static constexpr std::array<Piece, 4> blackPieces = {BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN};
        const auto& usedPieces = (color == Color::White ? whitePieces : blackPieces);

        static constexpr uint64_t rank8 = 0x00000000000000FFULL;
        static constexpr uint64_t rank7 = 0x000000000000FF00ULL;
        static constexpr uint64_t rank2 = 0x00FF000000000000ULL;
        static constexpr uint64_t rank1 = 0xFF00000000000000ULL;

        const uint64_t leftAttacks = color == Color::White ? notH : notA;
        const uint64_t rightAttacks = color == Color::White ? notA : notH;
        const uint64_t enPassantMask = enPassantSquare.has_value() ? 1ULL << shiftValue(enPassantSquare.value()) : 0ULL;
        const uint64_t pawnBoard = at(color == Color::White ? WHITE_PAWN : BLACK_PAWN);
        const uint64_t enemyBoard = color == Color::White ? at(Color::Black) : at(Color::White);
        const uint64_t empty = ~(at(Color::Black) | at(Color::White));
        const uint64_t lastRank = color == Color::White ? rank8 : rank1;
        const uint64_t doubleMoveRank = color == Color::White ? rank2 : rank7;

        const uint64_t combinedForwardMoves = shift<color>(pawnBoard, 8) & empty;
        const uint64_t combinedLeftCaptures = shift<color>(pawnBoard, 7) & leftAttacks & enemyBoard;
        const uint64_t combinedRightCaptures = shift<color>(pawnBoard, 9) & rightAttacks & enemyBoard;


        uint64_t singles = combinedForwardMoves & ~lastRank;
        uint64_t doubles = shift<color>(singles, 8) & empty & shift<color>(doubleMoveRank, 16);
        uint64_t capLeft = combinedLeftCaptures & ~lastRank;
        uint64_t capRight = combinedRightCaptures & ~lastRank;

        uint64_t promotion = combinedForwardMoves & lastRank;
        uint64_t promotionCapLeft = combinedLeftCaptures & lastRank;
        uint64_t promotionCapRight = combinedRightCaptures & lastRank;

        uint64_t enPassantLeft = shift<color>(pawnBoard, 7) & leftAttacks & enPassantMask;
        uint64_t enPassantRight = shift<color>(pawnBoard, 9) & rightAttacks & enPassantMask;

        auto emit = [&](uint64_t bb, const int offset, const int type=1) {
            while (bb) {
                const int to = pop_lsb(bb);
                const int from = to + offset;
                if (type == 2) {
                    moves.push_back(Move::enPassantCaptureMove(positionFromBitIndex(from),
                                                   positionFromBitIndex(to)));
                }
                else if (type == 3) {
                    BoardPosition end = positionFromBitIndex(to);
                    const int enPassantSquareY = end.y - (color == Color::White ? -1 : 1);
                    moves.push_back(Move::doublePawnMove(positionFromBitIndex(from), end, {end.x, enPassantSquareY}));
                }
                else {
                    moves.push_back(Move::standardMove(positionFromBitIndex(from),
                                                   positionFromBitIndex(to)));
                }
            }
        };

        auto emitPromotion = [&](uint64_t bb, const int offset) {
            while (bb) {
                const int to = pop_lsb(bb);
                const int from = to + offset;
                for (const Piece promoteTo : usedPieces) {
                    moves.push_back(Move::promotionMove(positionFromBitIndex(from), positionFromBitIndex(to), promoteTo));
                }
            }
        };

        emit(singles, color == Color::White ? 8 : -8);
        emit(doubles, color == Color::White ? 16 : -16, 3);
        emit(capLeft, color == Color::White ? 7 : -7);
        emit(capRight, color == Color::White ? 9 : -9);
        emit(enPassantLeft, color == Color::White ? 7 : -7, 2);
        emit(enPassantRight, color == Color::White ? 9 : -9, 2);
        emitPromotion(promotion, color == Color::White ? 8 : -8);
        emitPromotion(promotionCapLeft, color == Color::White ? 7 : -7);
        emitPromotion(promotionCapRight, color == Color::White ? 9 : -9);
    }

    template<Color color>
    static uint64_t shift(const uint64_t bb, const int amount) {
        return color == Color::Black ? (bb << amount) : (bb >> amount);
    }


    bool operator==(const BitBoardManager& other) const {
        return board == other.board && colorBoard == other.colorBoard;
    }
    bool operator!=(const BitBoardManager& other) const {
        return !operator==(other);
    }

private:
    std::array<uint64_t, 12> board{};
    std::array<uint64_t, 2> colorBoard{};

    static constexpr uint64_t notA = 0x7f7f7f7f7f7f7f7fULL;
    static constexpr uint64_t notH = 0xfefefefefefefefeULL;

    uint64_t& at_mut(const Color color) {
        return colorBoard[static_cast<size_t>(color)];
    }

    uint64_t& at_mut(const Pieces::Piece piece) {
        return board[indexOf(piece)];
    }

    static size_t indexOf(const Pieces::Piece piece) {
        switch (piece) {
            case Pieces::WHITE_PAWN:   return 0;
            case Pieces::WHITE_KNIGHT: return 1;
            case Pieces::WHITE_BISHOP: return 2;
            case Pieces::WHITE_ROOK:   return 3;
            case Pieces::WHITE_QUEEN:  return 4;
            case Pieces::WHITE_KING:   return 5;

            case Pieces::BLACK_PAWN:   return 6;
            case Pieces::BLACK_KNIGHT: return 7;
            case Pieces::BLACK_BISHOP: return 8;
            case Pieces::BLACK_ROOK:   return 9;
            case Pieces::BLACK_QUEEN:  return 10;
            case Pieces::BLACK_KING:   return 11;

            case Pieces::EMPTY:
            default:
                throw std::invalid_argument("EMPTY has no bitboard index");
        }
    }

    void add(const Pieces::Piece piece, const int y, const int x) {
        const uint64_t mask = (1ULL << shiftValue(y, x));
        at_mut(piece) |= mask;
        at_mut(Pieces::piece_color(piece)) |= mask;
    }

    void remove(const Pieces::Piece piece, const int y, const int x) {
        const uint64_t mask = (1ULL << shiftValue(y, x));
        at_mut(piece) &= ~mask;
        at_mut(Pieces::piece_color(piece)) &= ~mask;
    }

    static int shiftValue(const BoardPosition &pos) {
        return shiftValue(pos.y, pos.x);
    }
    static int shiftValue(const int y, const int x) {
        return y * BOARD_SIZE + x;
    }

    int pop_lsb(uint64_t &bb) const {
#if defined(_MSC_VER)
        unsigned long index;
        _BitScanForward64(&index, bb);
        int sq = (int)index;
#else
        int sq = __builtin_ctzll(bb);
#endif

        bb &= bb - 1;

        return sq;
    }

    static BoardPosition positionFromBitIndex(const int location) {
        return {location % BOARD_SIZE, location / BOARD_SIZE};
    }

    static void printBitboard(const uint64_t bb, bool flat=false) {
        std::cout << "\n";
        for (int rank = 0; rank < BOARD_SIZE; rank++) {
            for (int file = 0; file < BOARD_SIZE; file++) {
                int sq = rank * 8 + file;
                uint64_t mask = 1ULL << sq;

                std::cout << ((bb & mask) ? "1 " : ". ");
            }
            if (!flat) {
                std::cout << "\n";
            }
        }
        std::cout << "\n";
    }
};
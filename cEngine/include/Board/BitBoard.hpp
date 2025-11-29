//
// Created by Kiril on 11/23/2025.
//

#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include <ModuleOnly/Move.hpp>
#include "ModuleOnly/Utils.hpp"
#include "ModuleOnly/Enums.hpp"


struct MoveLookup {
    struct Entry {
        uint64_t blockerMask;
        uint64_t moves;
        bool operator<(const Entry& other) const {
            return blockerMask < other.blockerMask;
        }
    };

    std::vector<Entry> table;

    MoveLookup() {
        table.reserve(4096);
    }

    void add(uint64_t key, uint64_t val) {
        table.push_back({key, val});
    }
    void optimize() {
        std::sort(table.begin(), table.end());
    }

    uint64_t get(uint64_t key) const {
        Entry searchObj = {key, 0};
        auto it = std::lower_bound(table.begin(), table.end(), searchObj);

        if (it != table.end() && it->blockerMask == key) {
            return it->moves;
        }
        throw std::invalid_argument("No such entry");
    }
};

class BitBoard {
public:
    explicit BitBoard(const std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> &inp);

    void initKnightMasks();
    void initSlidingMasks(PieceType type);

    /**
     * Update the bitboard with a certain move.
     * @param mv Move played
     * @param startContent the contents of the mv.start when the move was played
     * @param endContent the contents of mv.end when the move was played
     */
    void move(const Move& mv, Pieces::Piece startContent, Pieces::Piece endContent);
    void undoMove(const Move& mv, Pieces::Piece movedPiece, Pieces::Piece overwrittenPiece, Color activeColor);

    static uint64_t attackMaskFor(PieceType pieceType, int pos, uint64_t board=0);
    static std::vector<uint64_t> getBlockerBitBoard(uint64_t attackMask);

    template<Color color, PieceType type>
    void addSlidingMoves(std::vector<Move>& moves) const {
        uint64_t pieces = at(Pieces::make_piece(color, type));
        while (pieces) {
            const int start = pop_lsb(pieces);

            uint64_t realMoves;
            if (type == PieceType::Queen) {
                realMoves = getRealMoves<color, PieceType::Rook>(start);
                realMoves |= getRealMoves<color, PieceType::Bishop>(start);
            } else {
                realMoves = getRealMoves<color, type>(start);
            }

            while (realMoves) {
                const int end = pop_lsb(realMoves);
                moves.push_back(Move::standardMove(positions[start], positions[end]));
            }
        }
    }

    template<Color color, PieceType type>
    [[nodiscard]] uint64_t getRealMoves(const int start) const {
        const std::array<uint64_t, SQUARE_COUNT>* pieceKeys = nullptr;
        const std::array<MoveLookup, SQUARE_COUNT>* moveMasks = nullptr;
        if (type == PieceType::Rook) {
            pieceKeys = &rookKeys;
            moveMasks = &rookMoves;
        } else if (type == PieceType::Bishop) {
            pieceKeys = &bishopKeys;
            moveMasks = &bishopMoves;
        } else {
            throw std::invalid_argument("Invalid piece type");
        }
        const uint64_t friendly = at(color);
        const uint64_t enemy = at(color == Color::White ? Color::Black : Color::White);
        const uint64_t occupancyKey = (*pieceKeys)[start] & (friendly | enemy);
        uint64_t realMoves = (*moveMasks)[start].get(occupancyKey);
        return realMoves & ~friendly;
    }


    template<Color color>
    void addKnightMoves(std::vector<Move>& moves) const {
        const Pieces::Piece piece = color == Color::White ? Pieces::WHITE_KNIGHT : Pieces::BLACK_KNIGHT;
        const uint64_t friendly = at(color);

        uint64_t knights = at(piece);
        while (knights) {
            const int start = pop_lsb(knights);
            uint64_t possibleMoves = knightMoves[start];
            possibleMoves &= ~friendly;
            while (possibleMoves) {
                const int end = pop_lsb(possibleMoves);
                moves.push_back(Move::standardMove(positions[start], positions[end]));
            }
        }
    }

    template<Color color>
    void addKingMoves(const int castleRights, std::vector<Move>& moves) const {
        const Pieces::Piece piece = color == Color::White ? Pieces::WHITE_KING : Pieces::BLACK_KING;

        uint64_t kingPos = at(piece);
        const BoardPosition kingStart = positions[pop_lsb(kingPos)];
        if (castleAllowed(color, CastleType::SHORT, castleRights)) {
            constexpr uint64_t shortMask = color == Color::White ? 0x6000000000000000 : 0x60;
            const BoardPosition end = color == Color::White ? BoardPosition{6, 7} : BoardPosition{6, 0};

            if ((shortMask & (at(Color::White) | at(Color::Black))) == 0) {
                moves.push_back(Move::castleMove(kingStart, end, CastleType::SHORT));
            }
        }
        if (castleAllowed(color, CastleType::LONG, castleRights)) {
            constexpr uint64_t longMask = color == Color::White ? 0xe00000000000000 : 0xe;
            const BoardPosition end = color == Color::White ? BoardPosition{2, 7} : BoardPosition{2, 0};

            if ((longMask & (at(Color::White) | at(Color::Black))) == 0) {
                moves.push_back(Move::castleMove(kingStart, end, CastleType::LONG));
            }
        }

        kingPos = at(piece);
        const uint64_t friendly = at(color);
        uint64_t kingMoves = 0;
        kingMoves ^= (kingPos & notA) << 1;

        kingMoves ^= (kingPos & notH) >> 1;

        kingMoves ^= (kingPos & ~rank8) >> 8;
        kingMoves ^= (kingPos & ~rank1) << 8;
        kingMoves ^= (kingPos & (notA & ~rank8)) >> 7;
        kingMoves ^= (kingPos & (notA & ~rank1)) << 9;
        kingMoves ^= (kingPos & (notH & ~rank8)) >> 9;
        kingMoves ^= (kingPos & (notH & ~rank1)) << 7;

        kingMoves &= ~friendly;

        while (kingMoves) {
            moves.push_back(Move::standardMove(kingStart, positions[pop_lsb(kingMoves)]));
        }
    }

    template<Color color>
    void addPawnMoves(const std::optional<BoardPosition> enPassantSquare, std::vector<Move>& moves) const {
        using namespace Pieces;
        static constexpr std::array<Piece, 4> whitePieces = {WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN};
        static constexpr std::array<Piece, 4> blackPieces = {BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN};
        const auto& usedPieces = (color == Color::White ? whitePieces : blackPieces);

        static constexpr uint64_t rank7 = 0x000000000000FF00ULL;
        static constexpr uint64_t rank2 = 0x00FF000000000000ULL;

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

                BoardPosition start = positions[from];
                BoardPosition end = positions[to];

                if (type == 2) {
                    moves.push_back(Move::enPassantCaptureMove(start, end));
                }
                else if (type == 3) {
                    const int enPassantSquareY = end.y - (color == Color::White ? -1 : 1);
                    moves.push_back(Move::doublePawnMove(start, end, {end.x, enPassantSquareY}));
                }
                else {
                    moves.push_back(Move::standardMove(start,end));
                }
            }
        };

        auto emitPromotion = [&](uint64_t bb, const int offset) {
            while (bb) {
                const int to = pop_lsb(bb);
                const int from = to + offset;
                for (const Piece promoteTo : usedPieces) {
                    moves.push_back(Move::promotionMove(positions[from], positions[to], promoteTo));
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

    template<Color color>
    static uint64_t shift(const uint64_t bb, const int amount) {
        return color == Color::Black ? (bb << amount) : (bb >> amount);
    }

    bool operator==(const BitBoard& other) const {
        return board == other.board && colorBoard == other.colorBoard;
    }
    bool operator!=(const BitBoard& other) const {
        return !operator==(other);
    }

private:
    std::array<uint64_t, 12> board{};
    std::array<uint64_t, 2> colorBoard{};
    std::array<uint64_t, SQUARE_COUNT> knightMoves{};
    std::array<uint64_t, SQUARE_COUNT> rookKeys{};
    std::array<MoveLookup, SQUARE_COUNT> rookMoves{};
    std::array<uint64_t, SQUARE_COUNT> bishopKeys{};
    std::array<MoveLookup, SQUARE_COUNT> bishopMoves{};
    std::array<BoardPosition, SQUARE_COUNT> positions{};

    static constexpr uint64_t notA = 0x7f7f7f7f7f7f7f7fULL;
    static constexpr uint64_t notH = 0xfefefefefefefefeULL;
    static constexpr uint64_t rank1 = 0xFF00000000000000ULL;
    static constexpr uint64_t rank8 = 0x00000000000000FFULL;
    static constexpr uint64_t noEdges = notA & notH & ~rank1 & ~rank8;

    static size_t indexOf(Pieces::Piece piece);
    static int pop_lsb(uint64_t &bb);
    static void printBitboard(uint64_t bb, bool flat=false);

    uint64_t& at_mut(const Color color) {
        return colorBoard[static_cast<size_t>(color)];
    }

    uint64_t& at_mut(const Pieces::Piece piece) {
        return board[indexOf(piece)];
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
};
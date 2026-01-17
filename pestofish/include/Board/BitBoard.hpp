//
// Created by Kiril on 11/23/2025.
//

#pragma once
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include <ModuleOnly/Move.hpp>
#include "ModuleOnly/Utils.hpp"
#include "ModuleOnly/Enums.hpp"

struct MagicEntry {
    uint64_t keyMask;
    uint64_t magic;
    int shift;
    int offset;

    int64_t getIndexFor(const uint64_t blockerMask) const {
        const uint64_t index = (blockerMask * this->magic) >> shift;
        return index;
    }

    template<class Archive>
    void serialize(Archive& ar)
    {
        ar(keyMask, magic, shift, offset);
    }
};

struct Magics {
    explicit Magics() {}

    explicit Magics(const int size) {
        table.reserve(size);
    }

    std::vector<uint64_t> table;
    std::array<MagicEntry, SQUARE_COUNT> magics{};

    inline uint64_t getMoves(const int pos, uint64_t occupancy) const {
        occupancy &= magics[pos].keyMask;
        occupancy *= magics[pos].magic;
        occupancy >>= magics[pos].shift;
        return table[magics[pos].offset + occupancy];
    }

    template<class Archive>
    void serialize(Archive& ar)
    {
        ar(table, magics);
    }
};

class BitBoard {
public:
    explicit BitBoard(const std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> &inp);

    void initKnightMasks();

    /**
     * Update the bitboard with a certain move.
     * @param mv Move played
     * @param startContent the contents of the mv.start when the move was played
     * @param endContent the contents of mv.end when the move was played
     */
    void move(const Move& mv, Pieces::Piece startContent, Pieces::Piece endContent);
    void undoMove(const Move& mv, Pieces::Piece movedPiece, Pieces::Piece overwrittenPiece, Color activeColor);

    template<Color color>
    [[nodiscard]] bool inCheck() const;
    template<Color color>
    bool isLegalMove(const Move& mv, Pieces::Piece startContent, Pieces::Piece endContent);

    /**
     * Get the King attack mask (it's 9 moves). Castling is not calculated
     */
    template<Color color>
    [[nodiscard]] uint64_t getKingAttackMask() const;
    template<Color color>
    void addKingMoves(int castleRights, std::vector<Move>& moves) const;

    template<Color color, PieceType type>
    [[nodiscard]] uint64_t getSlidingAttackMask() const;
    template<Color color, PieceType type>
    void addSlidingMoves(std::vector<Move>& moves) const;

    template<Color color>
    [[nodiscard]] uint64_t getKnightAttackMask() const;
    template<Color color>
    void addKnightMoves(std::vector<Move>& moves) const;

    template<Color color>
    [[nodiscard]] uint64_t getPawnAttackMask() const;
    template<Color color>
    void addPawnMoves(std::optional<BoardPosition> enPassantSquare, std::vector<Move>& moves) const;

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

    static Magics rookMagics;
    static Magics bishopMagics;
    static std::array<uint64_t, SQUARE_COUNT> knightMoves;
    static std::array<BoardPosition, SQUARE_COUNT> positions;
    static bool initialized;

    static constexpr uint64_t notA = 0x7f7f7f7f7f7f7f7fULL;
    static constexpr uint64_t notH = 0xfefefefefefefefeULL;
    static constexpr uint64_t rank1 = 0xFF00000000000000ULL;
    static constexpr uint64_t rank8 = 0x00000000000000FFULL;
    static constexpr uint64_t noEdges = notA & notH & ~rank1 & ~rank8;
    static constexpr uint64_t shortCastleWhite = 0x6000000000000000;
    static constexpr uint64_t shortCastleBlack = 0x60;
    static constexpr uint64_t longCastleWhite = 0xe00000000000000;
    static constexpr uint64_t longCastleBlack = 0xe;

    /**
     * Get the keymask for a sliding piece in a specific position.
     * That is, all the possible moves on an empty board, with the end moves removed.
     */
    static uint64_t keyMask(PieceType type, int square);
    /**
     * Get all the possible attacks for sliding piece in a specific position and with a specific board configuration
     */
    static uint64_t slidingAttackMaskFor(PieceType pieceType, int pos, uint64_t board=0);
    /**
     * Get all the possible combinations of blockers for a certain attack mask
     */
    static std::vector<uint64_t> getBlockerBitBoard(uint64_t attackMask);
    /**
     * get the index for a specific piece within the per piece list of bitboards.
     */
    static size_t indexOf(const Pieces::Piece piece) {
        return (piece >> 3) * 6 + ((piece & 0b111) - 1);
    }
    /**
     * Remove the least significant bit and return the value.
     */
    static int pop_lsb(uint64_t &bb);

    template<Color color, PieceType type>
    [[nodiscard]] uint64_t getRealMoves(int start) const;

    template<Color color>
    [[nodiscard]] uint64_t getAttackMask() const;

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

    // FINDING MAGIC
    static void loadMagics();
    static void findMagics(PieceType type, Magics& entry, bool verbose=false);
    static std::pair<MagicEntry, std::vector<uint64_t>> findMagic(PieceType type, int pos);
    static bool fillAndValidateMagic(
        PieceType type, int bits, int pos,
        const std::vector<uint64_t>& blockerMasks, const MagicEntry& magicEntry, std::vector<uint64_t>& movesOut
    );

    // DEBUG
    /**
     * Pretty print a specific bitboard
     */
    static void printBitboard(uint64_t bb, bool flat=false);
};

#include "BitBoard.tpp"
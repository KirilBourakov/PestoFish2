//
// Created by Kiril on 2025-11-02.
//
#pragma once
#include <bit>
#include <memory>
#include <shared_mutex>
#include <vector>

#include "pestofish/Core/Move.hpp"
#include "pestofish/Core/ZobristHash.hpp"

// TODO: rewrite using atomics, storing a packed Move in u16/u32 instead of mutex. Or drop locks all together.
namespace Transposition {
    enum class CutoffType : char { UPPER_BOUND = 0, LOWER_BOUND = 1, EXACT = 2, INVALID = 3 };

    constexpr int BITS_AGE   = 8;
    constexpr int BITS_TYPE  = 2;
    constexpr int BITS_SCORE = 16;
    constexpr int BITS_DEPTH = 16;
    constexpr int BITS_MOVE  = 16;
    // (Remaining 14 bits are empty/padding)

    constexpr int SHIFT_AGE   = 0;
    constexpr int SHIFT_TYPE  = SHIFT_AGE + BITS_AGE;
    constexpr int SHIFT_SCORE = SHIFT_TYPE + BITS_TYPE;
    constexpr int SHIFT_DEPTH = SHIFT_SCORE + BITS_SCORE;
    constexpr int SHIFT_MOVE  = SHIFT_DEPTH + BITS_DEPTH;

    // 3. Raw Masks (The bit pattern before shifting)
    constexpr uint64_t MASK_AGE   = (1ULL << BITS_AGE) - 1;
    constexpr uint64_t MASK_TYPE  = (1ULL << BITS_TYPE) - 1;
    constexpr uint64_t MASK_SCORE = (1ULL << BITS_SCORE) - 1;
    constexpr uint64_t MASK_DEPTH = (1ULL << BITS_DEPTH) - 1;
    constexpr uint64_t MASK_MOVE  = (1ULL << BITS_MOVE) - 1;

    constexpr int VALUES = 4;

    inline Move ttEntryMove(const uint64_t& data) {
        return Move{static_cast<uint16_t>((data >> SHIFT_MOVE) & MASK_MOVE)};
    }

    inline int16_t ttEntryDepth(const uint64_t& data) {
        return static_cast<int16_t>((data >> SHIFT_DEPTH) & MASK_DEPTH);
    }

    inline int16_t ttEntryScore(const uint64_t& data) {
        return static_cast<int16_t>((data >> SHIFT_SCORE) & MASK_SCORE);
    }

    inline CutoffType ttEntryCutType(const uint64_t& data) {
        return static_cast<CutoffType>((data >> SHIFT_TYPE) & MASK_TYPE);
    }

    inline uint8_t ttEntryAge(const uint64_t& data) {
        return static_cast<uint8_t>((data >> SHIFT_AGE) & MASK_AGE);
    }

    struct Entry {
        Entry();

        static uint64_t pack(Move bestMove, int16_t depth, int16_t score, CutoffType cutoffType, unsigned short age);
        void save(int i, uint64_t checksumIn, uint64_t packedData);

        /**
         * 0 -> recency based. 1-3 -> depth based.
         * Each entry:
         * [6 bits empty] [16 bits move] [16 bits depth] [16 bits score] [2 bits cutoff type] [8 bits age]
         */
        std::array<std::atomic<uint64_t>, VALUES> data;
        std::array<std::atomic<uint64_t>, VALUES> checksum;

        uint64_t getKey(const int i) const { return checksum[i]; }
    };

    constexpr int tableSizeMb = 128 * 2;
    constexpr size_t rawEntries = tableSizeMb * 1000000ULL / sizeof(Entry);
    constexpr size_t tableSizeEntries = std::bit_ceil(rawEntries);
    constexpr int16_t quiescence_depth = -1;

    class TranspositionTable {
    public:
        TranspositionTable()
            : table(std::vector<Entry>(tableSizeEntries))
        {}

        bool lookup(uint64_t key, uint64_t& entry_out) const;

        void insert(uint64_t key, const Move& bestMove, int16_t depth, int16_t score, CutoffType cutoffType,
                    unsigned char age);

    private:
        std::vector<Entry> table;
        int ageOverride = 4;
    };
}

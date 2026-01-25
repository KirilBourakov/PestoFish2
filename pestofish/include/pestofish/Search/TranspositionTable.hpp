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
        Entry() : data(static_cast<uint64_t>(CutoffType::INVALID) << SHIFT_TYPE), checksum(0) {}
        Entry(const uint64_t checksum, const uint64_t data)
            : data(data)
            , checksum(checksum)
        {}

        static uint64_t pack(Move bestMove, int16_t depth, int16_t score, CutoffType cutoffType, unsigned short age) {
            uint64_t data = static_cast<uint64_t>(bestMove.getMoveEncoding()) << SHIFT_MOVE;
            data |= static_cast<uint64_t>(static_cast<uint16_t>(depth)) << SHIFT_DEPTH;
            data |= static_cast<uint64_t>(static_cast<uint16_t>(score)) << SHIFT_SCORE;
            data |= static_cast<uint64_t>(static_cast<uint8_t>(cutoffType)) << SHIFT_TYPE;
            data |= static_cast<uint64_t>(static_cast<uint8_t>(age)) << SHIFT_AGE;
            return data;
        }

        void save(uint64_t key, uint64_t packedData) {
            data.store(packedData, std::memory_order_relaxed);
            checksum.store(key ^ packedData, std::memory_order_relaxed);
        }

        /**
         * [6 bits empty] [16 bits move] [16 bits depth] [16 bits score] [2 bits cutoff type] [8 bits age]
         */
        std::atomic<uint64_t> data;
        std::atomic<uint64_t> checksum;

        uint64_t getKey() const { return checksum; }
    };

    struct PaddedMutex {
        alignas(64) std::shared_mutex m;
    };

    constexpr int tableSizeMb = 128;
    constexpr size_t rawEntries = tableSizeMb * 1000000ULL / sizeof(Entry);
    constexpr size_t tableSizeEntries = std::bit_ceil(rawEntries);
    constexpr int16_t quiescence_depth = -1;

    using table = std::vector<Entry>;

    class TranspositionTable {
    public:
        TranspositionTable()
            : depthPreferred(std::make_unique<table>(tableSizeEntries))
            , alwaysReplace(std::make_unique<table>(tableSizeEntries))
        {}

        bool lookup(uint64_t key, uint64_t& entry_out) const;

        void insert(uint64_t key, const Move& bestMove, int16_t depth, int16_t score, CutoffType cutoffType,
                    unsigned char age);

    private:
        std::unique_ptr<table> depthPreferred;
        std::unique_ptr<table> alwaysReplace;
        int ageOverride = 4;
    };
}

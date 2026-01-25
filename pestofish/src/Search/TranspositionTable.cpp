//
// Created by Kiril on 1/24/2026.
//

#include "pestofish/Search/TranspositionTable.hpp"

Transposition::Entry::Entry() {
    for (int i = 0; i < VALUES; i++) {
        data[i] = (static_cast<uint64_t>(CutoffType::INVALID) << SHIFT_TYPE);
    }
}

uint64_t Transposition::Entry::pack(Move bestMove, int16_t depth, int16_t score, CutoffType cutoffType, unsigned short age) {
    uint64_t data = static_cast<uint64_t>(bestMove.getMoveEncoding()) << SHIFT_MOVE;
    data |= static_cast<uint64_t>(static_cast<uint16_t>(depth)) << SHIFT_DEPTH;
    data |= static_cast<uint64_t>(static_cast<uint16_t>(score)) << SHIFT_SCORE;
    data |= static_cast<uint64_t>(static_cast<uint8_t>(cutoffType)) << SHIFT_TYPE;
    data |= static_cast<uint64_t>(static_cast<uint8_t>(age)) << SHIFT_AGE;
    return data;
}

void Transposition::Entry::save(const int i, const uint64_t checksumIn, const uint64_t packedData) {
    data[i].store(packedData, std::memory_order_relaxed);
    checksum[i].store(checksumIn, std::memory_order_relaxed);
}

// Write vector into large pages (look into prefetching)
// Fix searching issues - re examine psedo code in chess programming, have gemeni generate also and reason about differences
// Explore finding better magics.
bool Transposition::TranspositionTable::lookup(const uint64_t key, uint64_t& entry_out) const {
    const unsigned long long index = key & (tableSizeEntries - 1);

    const Entry& entry = table[index];

    for (int i = 0; i < VALUES; i++) {
        const uint64_t data = entry.data[i].load(std::memory_order_relaxed);
        const uint64_t checksum = entry.checksum[i].load(std::memory_order_relaxed);
        if (const uint64_t entryKey = checksum ^ data; entryKey == key) {
            entry_out = data;
            return true;
        }
    }

    return false;
}

void Transposition::TranspositionTable::insert(
    const uint64_t key, const Move& bestMove, const int16_t depth, const int16_t score, const CutoffType cutoffType, const unsigned char age) {
    const unsigned long long index = key & (tableSizeEntries - 1);

    const uint64_t data = Entry::pack(bestMove, depth, score, cutoffType, age);
    const uint64_t checksum = key ^ data;

    Entry& entry = table[index];

    for (int i = 0; i < VALUES; i++) {
        const uint64_t storedData = entry.data[i].load(std::memory_order_relaxed);
        const uint64_t storedChecksum = entry.checksum[i].load(std::memory_order_relaxed);
        if ((storedChecksum ^ storedData) == key || ttEntryCutType(storedData) == CutoffType::INVALID) {
            entry.save(i, checksum, data);
            return;
        }
    }

    int best_slot = 0;
    int min_depth = 32000;
    for (int i = 1; i < VALUES; i++) {
        const uint64_t storedData = entry.data[i].load(std::memory_order_relaxed);
        const int stored_depth = ttEntryDepth(storedData);
        if (stored_depth < min_depth) {
            min_depth = stored_depth;
            best_slot = i;
        }
    }
    if (depth >= min_depth) {
        entry.save(best_slot, checksum, data);
    } else {
        entry.save(0, checksum, data);
    }
}
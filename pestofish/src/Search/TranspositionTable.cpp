//
// Created by Kiril on 1/24/2026.
//

#include "pestofish/Search/TranspositionTable.hpp"
// TODO: fix transposition table by clustering together lines (clusters of 4 for cache line)
// Write vector into large pages (look into prefetching)
// Fix searching issues - re examine psedo code in chess programming, have gemeni generate also and reason about differences
// Explore finding better magics.
bool Transposition::TranspositionTable::lookup(const uint64_t key, uint64_t& entry_out) const {
    const unsigned long long index = key & (tableSizeEntries - 1);

    Entry& depthEntry = (*depthPreferred)[index];
    uint64_t data = depthEntry.data.load(std::memory_order_relaxed);
    uint64_t checksum = depthEntry.checksum.load(std::memory_order_relaxed);

    if (ttEntryCutType(data) != CutoffType::INVALID) {
        uint64_t entryKey = checksum ^ data;
        if (entryKey == key) {
            entry_out = data;
            return true;
        }
    }

    const Entry& alwaysEntry = (*alwaysReplace)[index];
    uint64_t alwaysData = alwaysEntry.data.load(std::memory_order_relaxed);
    uint64_t alwaysChecksum = alwaysEntry.checksum.load(std::memory_order_relaxed);

    if (ttEntryCutType(alwaysData) != CutoffType::INVALID) {
        uint64_t entryKey = alwaysChecksum ^ alwaysData;
        if (entryKey == key) {
            entry_out = alwaysData;
            return true;
        }
    }

    return false;
}

void Transposition::TranspositionTable::insert(
    const uint64_t key, const Move& bestMove, const int16_t depth, const int16_t score, const CutoffType cutoffType, const unsigned char age) {
    const unsigned long long index = key & (tableSizeEntries - 1);
    const uint64_t data = Entry::pack(bestMove, depth, score, cutoffType, age);

    const Entry& oldEntry = (*depthPreferred)[index];
    uint64_t oldEntryData = oldEntry.data.load(std::memory_order_relaxed);
    if (ttEntryCutType(oldEntryData) == CutoffType::INVALID ||
        ttEntryDepth(oldEntryData) < ttEntryDepth(data) ||
        ttEntryAge(data) - ttEntryAge(oldEntryData) >= ageOverride
    ) {
        (*depthPreferred)[index].save(key, data);
        return;
    }
    (*alwaysReplace)[index].save(key, data);
}
//
// Created by Kiril on 2025-11-02.
//
module;
#include <concurrent_vector.h>
#include <optional>
#include <bit>

export module TranspositionTable;
import Move;
import ZobristHash;

// TODO: thread safety, allocate tables on the heap.
export namespace Transposition {
    enum class CutoffType : unsigned short {
        UPPER_BOUND = 0,
        LOWER_BOUND = 1,
        EXACT = 2,
    };

    struct alignas(64) Entry {
        unsigned short key;
        Move bestMove;
        unsigned short depth;
        int score;
        CutoffType cutoffType;
        unsigned short age;
    };

    constexpr int tableSizeMb = 128;
    constexpr unsigned long long rawEntries = tableSizeMb * 1000000ULL / sizeof(Entry);
    constexpr unsigned long long tableSizeEntries = std::bit_ceil(rawEntries);

    using table = std::array<std::optional<Entry>, tableSizeEntries>;
    class TranspositionTable {
    public:
        bool lookup(const u64 key, int& score, Move& moveOut, CutoffType& cutoffOut) {
            const unsigned long long index = key & (tableSizeEntries - 1);
            auto upperBits = static_cast<unsigned short>(key >> 16);

            std::optional<Entry> entry = std::nullopt;
            if ((*depthPreferred)[index].has_value()) {
                if ((*depthPreferred)[index].value().key == upperBits) {
                    entry = (*depthPreferred)[index].value();
                }
            }
            if (!entry.has_value() && (*alwaysReplace)[index].has_value()) {
                if ((*alwaysReplace)[index].value().key == upperBits) {
                    entry = (*alwaysReplace)[index].value();
                }
            }

            if (entry.has_value()) {
                score = entry->score;
                moveOut = entry->bestMove;
                cutoffOut = entry->cutoffType;
                return true;
            }
            return false;
        }

        void insert(
            const u64 key, const Move &bestMove, const unsigned short depth,
            const int score, const CutoffType cutoffType, const unsigned short age
        ) const {
            const unsigned long long index = key & (tableSizeEntries - 1);
            const auto upperBits = static_cast<unsigned short>(key >> 16);

            Entry newEntry = {upperBits, bestMove, depth, score, cutoffType, age};

            if (!(*depthPreferred)[index].has_value() || (*depthPreferred)[index].value().depth < newEntry.depth) {
                (*depthPreferred)[index] = newEntry;
                return;
            }
            (*alwaysReplace)[index] = newEntry;
        }

    private:
        std::unique_ptr<table> depthPreferred;
        std::unique_ptr<table> alwaysReplace;
    };
}
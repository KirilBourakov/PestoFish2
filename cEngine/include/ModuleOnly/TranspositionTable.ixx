//
// Created by Kiril on 2025-11-02.
//
module;
#include <concurrent_vector.h>
#include <optional>

export module TranspositionTable;
import Move;
import ZobristHash;

// TODO: thread safety, allocate tables on the heap. tableSizeEntries must be a power of two, or switch to modules to get index. Start using age to replace also
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
    constexpr unsigned long long tableSizeEntries = tableSizeMb * 1e6 / sizeof(Entry);

    class TranspositionTable {
    public:
        bool lookup(const u64 key, int& score, Move& moveOut, CutoffType& cutoffOut) {
            const unsigned long long index = key & (tableSizeEntries - 1);
            auto upperBits = static_cast<unsigned short>(key >> 16);

            std::optional<Entry> entry = std::nullopt;
            if (depthPreferred.at(index).has_value()) {
                if (depthPreferred.at(index).value().key == upperBits) {
                    entry = depthPreferred.at(index).value();
                }
            }
            if (!entry.has_value() && alwaysReplace.at(index).has_value()) {
                if (alwaysReplace.at(index).value().key == upperBits) {
                    entry = alwaysReplace.at(index).value();
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
        ) {
            const unsigned long long index = key & (tableSizeEntries - 1);
            const auto upperBits = static_cast<unsigned short>(key >> 16);

            Entry newEntry = {upperBits, bestMove, depth, score, cutoffType, age};

            if (!depthPreferred.at(index).has_value() || depthPreferred.at(index).value().depth < newEntry.depth) {
                depthPreferred[index] = newEntry;
                return;
            }
            alwaysReplace[index] = newEntry;
        }

    private:
        std::array<std::optional<Entry>, tableSizeEntries> depthPreferred;
        std::array<std::optional<Entry>, tableSizeEntries> alwaysReplace;

    };
}
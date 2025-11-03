//
// Created by Kiril on 2025-11-02.
//
module;
#include <bit>
#include <memory>
#include <mutex>
#include <shared_mutex>

export module TranspositionTable;
import <array>;
import Move;
import ZobristHash;

export namespace Transposition {
    enum class CutoffType : unsigned short {
        UPPER_BOUND = 0,
        LOWER_BOUND = 1,
        EXACT = 2,
    };

    struct Entry {
        unsigned int key = 0;
        Move bestMove;
        unsigned short depth = 0;
        short score = 0;
        CutoffType cutoffType;
        unsigned char age = 0;

        bool has_value() const {
            return !(age == 0 && depth == 0 && key == 0);
        }
    };

    struct PaddedMutex {
        alignas(64) std::shared_mutex m;
    };

    constexpr int tableSizeMb = 128;
    constexpr unsigned long long rawEntries = tableSizeMb * 1000000ULL / sizeof(Entry);
    constexpr unsigned long long tableSizeEntries = std::bit_ceil(rawEntries);

    using table = std::array<Entry, tableSizeEntries>;
    class TranspositionTable {
    public:
        TranspositionTable() :
            depthPreferred(std::make_unique<table>()),
            alwaysReplace(std::make_unique<table>())
        {}

        bool lookup(const u64 key, int& score, Move& moveOut, CutoffType& cutoffOut) {
            const unsigned long long index = key & (tableSizeEntries - 1);
            const auto verificationKey = static_cast<unsigned int>(key >> 32);

            Entry entry{};

            std::shared_lock<std::shared_mutex> lock(mLocks[index % numClusters].m);

            Entry& depthEntry = (*depthPreferred)[index];
            if (depthEntry.key == verificationKey && depthEntry.has_value()) {
                score = depthEntry.score;
                moveOut = depthEntry.bestMove;
                cutoffOut = depthEntry.cutoffType;
                return true;
            }

            Entry& alwaysEntry = (*alwaysReplace)[index];
            if (alwaysEntry.key == verificationKey && alwaysEntry.has_value()) {
                score = alwaysEntry.score;
                moveOut = alwaysEntry.bestMove;
                cutoffOut = alwaysEntry.cutoffType;
                return true;
            }

            return false;
        }

        void insert(
            const u64 key, const Move &bestMove, const unsigned short depth,
            const int score, const CutoffType cutoffType, const unsigned char age
        ) {
            const unsigned long long index = key & (tableSizeEntries - 1);
            const auto verificationKey = static_cast<unsigned int>(key >> 32);

            Entry newEntry = {verificationKey, bestMove, depth, static_cast<short>(score), cutoffType, age};

            std::unique_lock<std::shared_mutex> lock(mLocks[index % numClusters].m);
            Entry& oldEntry = (*depthPreferred)[index];
            if (!oldEntry.has_value() || oldEntry.depth < newEntry.depth || newEntry.age - oldEntry.age >= ageOverride) {
                (*depthPreferred)[index] = newEntry;
                return;
            }
            (*alwaysReplace)[index] = newEntry;
        }

    private:
        // TODO: replace with calculated value based on table size
        static constexpr size_t numClusters = 256;

        std::unique_ptr<table> depthPreferred;
        std::unique_ptr<table> alwaysReplace;
        int ageOverride = 4;

        std::array<PaddedMutex, numClusters> mLocks;
    };
}
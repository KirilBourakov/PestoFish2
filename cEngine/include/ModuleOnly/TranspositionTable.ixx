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

// TODO: rewrite using atomics, storing a packed Move in u16/u32 instead of a Move
export namespace Transposition {
    enum class CutoffType : char {
        UPPER_BOUND = 0,
        LOWER_BOUND = 1,
        EXACT = 2,
        INVALID = 3
    };

    struct Entry {
        Entry() : key(), bestMove(), depth(), score(), cutoffType(CutoffType::INVALID), age() {}

        unsigned int key;
        Move bestMove;
        unsigned short depth;
        short score;
        CutoffType cutoffType;
        unsigned char age;

        [[nodiscard]] bool has_value() const {
            return cutoffType != CutoffType::INVALID;
        }
    };

    struct PaddedMutex {
        alignas(64) std::shared_mutex m;
    };

    constexpr int tableSizeMb = 128;
    constexpr size_t rawEntries = tableSizeMb * 1000000ULL / sizeof(Entry);
    constexpr size_t tableSizeEntries = std::bit_ceil(rawEntries);
    constexpr size_t numLocks = std::max<size_t>(64, tableSizeEntries / 4096);

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

            std::shared_lock<std::shared_mutex> lock(mLocks[index % numLocks].m);

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

            std::unique_lock<std::shared_mutex> lock(mLocks[index % numLocks].m);
            Entry& oldEntry = (*depthPreferred)[index];
            if (!oldEntry.has_value() || oldEntry.depth < newEntry.depth || newEntry.age - oldEntry.age >= ageOverride) {
                (*depthPreferred)[index] = newEntry;
                return;
            }
            (*alwaysReplace)[index] = newEntry;
        }

    private:
        std::unique_ptr<table> depthPreferred;
        std::unique_ptr<table> alwaysReplace;
        int ageOverride = 4;

        std::array<PaddedMutex, numLocks> mLocks;
    };
}
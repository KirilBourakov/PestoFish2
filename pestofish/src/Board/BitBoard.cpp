//
// Created by Kiril on 11/29/2025.
//
#include "Board/BitBoard.hpp"

#include <random>
#include <bit>
#include <filesystem>
#include <fstream>

#include <cereal/archives/binary.hpp>

#include "Resources/bishop.h"
#include "Resources/buffer.hpp"
#include "Resources/rooks.h"

Magics BitBoard::rookMagics;
Magics BitBoard::bishopMagics;
std::array<uint64_t, SQUARE_COUNT> BitBoard::knightMoves;
std::array<BoardPosition, SQUARE_COUNT> BitBoard::positions;
bool BitBoard::initialized = false;

BitBoard::BitBoard(const std::array<std::array<Pieces::Piece, BOARD_SIZE>, BOARD_SIZE> &inp)
{
    if (!initialized) {
        for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
            positions[i] = {i % BOARD_SIZE, i / BOARD_SIZE};
        }
        loadOrFindMagics();
        initKnightMasks();

        initialized = true;
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (inp[y][x] != Pieces::EMPTY) {
                insert(inp[y][x], y, x);
            }
        }
    }
}

void BitBoard::initKnightMasks() {
    const int offsets[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (int square = 0; square < 64; square++) {
        uint64_t b = 0;
        const int y = square / 8;
        const int x = square % 8;

        for (auto& offset : offsets) {
            const int ny = y + offset[0];
            const int nx = x + offset[1];
            if (ny >= 0 && ny < 8 && nx >= 0 && nx < 8) {
                b |= (1ULL << shiftValue(ny, nx));
            }
        }
        knightMoves[square] = b;
    }
}

void BitBoard::move(const Move& mv, const Pieces::Piece startContent, const Pieces::Piece endContent) {
    // remove piece
    if (endContent != Pieces::EMPTY) {
        remove(endContent, mv.end.y, mv.end.x);
    }

    // move
    remove(startContent, mv.start.y, mv.start.x);
    add(mv.promotedTo.value_or(startContent), mv.end.y, mv.end.x);

    // remove pawn behind, if en passant
    if (mv.enPassantCapture) {
        const Pieces::Piece captured = startContent == Pieces::WHITE_PAWN ? Pieces::BLACK_PAWN : Pieces::WHITE_PAWN;
        remove(captured, mv.start.y, mv.end.x);
    }

    // move rook when castling
    else if (mv.castle == CastleType::LONG) {
        const Pieces::Piece rook = startContent == Pieces::WHITE_KING ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
        remove(rook, mv.start.y, 0);
        add(rook, mv.start.y, mv.end.x+1);
    }

    else if (mv.castle == CastleType::SHORT) {
        const Pieces::Piece rook = startContent == Pieces::WHITE_KING ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
        remove(rook, mv.start.y, 7);
        add(rook, mv.start.y, mv.end.x-1);
    }
}

void BitBoard::undoMove(const Move& mv, const Pieces::Piece movedPiece, const Pieces::Piece overwrittenPiece, const Color activeColor) {
    if (mv.enPassantCapture) {
        add((activeColor == Color::White) ? Pieces::BLACK_PAWN : Pieces::WHITE_PAWN, mv.start.y, mv.end.x);
    }
    else if (mv.castle == CastleType::LONG) {
        const Pieces::Piece rook = (activeColor == Color::White) ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
        remove(rook, mv.start.y, mv.end.x+1);
        add(rook, mv.start.y, 0);
    }
    else if (mv.castle == CastleType::SHORT) {
        const Pieces::Piece rook = (activeColor == Color::White) ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
        remove(rook, mv.start.y, mv.end.x-1);
        add(rook, mv.start.y, 7);
    }

    remove(mv.promotedTo.value_or(movedPiece), mv.end.y, mv.end.x);
    add(movedPiece, mv.start.y, mv.start.x);

    if (overwrittenPiece != Pieces::EMPTY) {
        add(overwrittenPiece, mv.end.y, mv.end.x);
    }
}

std::vector<uint64_t> BitBoard::getBlockerBitBoard(const uint64_t attackMask) {
    std::vector<int> locations;
    uint64_t mask = attackMask;
    while (mask) {
        locations.push_back(pop_lsb(mask));
    }

    int numPatterns = 1 << locations.size();
    std::vector<uint64_t> patterns;
    patterns.assign(numPatterns, 0ULL);

    for (int patternIndex = 0; patternIndex < numPatterns; patternIndex++) {
        for (int bitIndex = 0; bitIndex < locations.size(); bitIndex++) {
            int bit = (patternIndex >> bitIndex) & 1;
            patterns[patternIndex] |= static_cast<uint64_t>(bit) << locations[bitIndex];
        }
    }
    return std::move(patterns);
}

uint64_t BitBoard::keyMask(const PieceType type, const int square) {
    const uint64_t attackMask = slidingAttackMaskFor(type, square);
    const int y = square / 8;
    const int x = square % 8;
    uint64_t endMask = ~0ULL;
    if (y != 0) {
        endMask &= ~rank8;
    }
    if (y != 7) {
        endMask &= ~rank1;
    }
    if (x != 7) {
        endMask &= notA;
    }
    if (x != 0) {
        endMask &= notH;
    }
    return attackMask & endMask;
}

uint64_t BitBoard::slidingAttackMaskFor(const PieceType pieceType, const int pos, const uint64_t board) {
    using moveSet = std::vector<std::pair<int, int>>;
    static const moveSet straight_diag = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    static const moveSet diag_dir = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    static const moveSet straight_dir = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    const moveSet* dirs;
    if (pieceType == PieceType::Queen) {
        dirs = &straight_diag;
    } else if (pieceType == PieceType::Bishop) {
        dirs = &diag_dir;
    } else if (pieceType == PieceType::Rook) {
        dirs = &straight_dir;
    } else {
        throw std::invalid_argument("Invalid direction");
    }

    const int start_y = pos / 8;
    const int start_x = pos % 8;

    uint64_t mask = 0;
    for (auto [dx, dy] : *dirs) {
        for (int i = 1; i < BOARD_SIZE; i++) {
            const int newY = start_y + i * dy;
            const int newX = start_x + i * dx;
            if (!inBounds(newX, newY)) {
                break;
            }
            mask |= (1ULL << shiftValue(newY, newX));

            if (((1ULL << shiftValue(newY, newX)) & board) != 0) {
                break;
            }
        }
    }
    return mask;
}

int BitBoard::pop_lsb(uint64_t &bb) {
    #if defined(_MSC_VER)
        unsigned long index;
        _BitScanForward64(&index, bb);
        int sq = (int)index;
    #else
        int sq = __builtin_ctzll(bb);
    #endif

    bb &= bb - 1;

    return sq;
}

void BitBoard::printBitboard(const uint64_t bb, const bool flat) {
    std::cout << "\n";
    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            int sq = rank * 8 + file;
            uint64_t mask = 1ULL << sq;

            std::cout << ((bb & mask) ? "1 " : ". ");
        }
        if (!flat) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}

void BitBoard::loadOrFindMagics() {
    auto rookbuf = MemBuf(resources_rooks_bin, resources_rooks_bin_len);
    std::istream rookStream(&rookbuf);
    cereal::BinaryInputArchive rookArch(rookStream);
    rookArch(rookMagics);

    auto bishopBuf = MemBuf(resources_bishops_bin, resources_bishops_bin_len);
    std::istream bishopStream(&bishopBuf);
    cereal::BinaryInputArchive bishopArch(bishopStream);
    bishopArch(bishopMagics);
}

void BitBoard::findMagics(const PieceType type, Magics& entry, bool verbose) {
    int offset = 0;
    for (int i = 0; i < SQUARE_COUNT; i++) {
        if (verbose) {
            std::cout << "Starting " << i << std::endl;
        }
        std::pair<MagicEntry, std::vector<uint64_t>> result = findMagic(type, i);

        MagicEntry magic = result.first;
        std::vector<uint64_t>& moves = result.second;
        magic.offset = offset;

        entry.magics[i] = magic;
        for (auto& move : moves) {
            entry.table.push_back(move);
            offset++;
        }

        if (verbose) {
            std::cout << "Finished " << i << std::endl;
        }
    }
}

std::pair<MagicEntry, std::vector<uint64_t>> BitBoard::findMagic(const PieceType type, const int pos) {
    static std::mt19937_64 rng{};
    static std::uniform_int_distribution<uint64_t> dist;

    const uint64_t key = keyMask(type, pos);
    const int bits = std::popcount(key);
    const int shift = 64 - bits;

    const std::vector<uint64_t> blockerMasks = getBlockerBitBoard(key);

    while (true) {
        const uint64_t magicCandidate = dist(rng) & dist(rng) & dist(rng);
        MagicEntry entry = {key, magicCandidate, shift, 0};
        std::vector<uint64_t> movesOut;

        if (fillAndValidateMagic(type, bits, pos, blockerMasks, entry, movesOut)) {
            return std::make_pair(entry, movesOut);
        }
    }
}

bool BitBoard::fillAndValidateMagic(
    const PieceType type,
    const int bits,
    const int pos,
    const std::vector<uint64_t>& blockerMasks,
    const MagicEntry& magicEntry,
    std::vector<uint64_t>& movesOut
) {

    std::vector<bool> filled;
    filled.assign(1ULL << bits, false);
    movesOut.assign(1ULL << bits, 0);

    for (const auto& blockerMask : blockerMasks) {
        const int index = magicEntry.getIndexFor(blockerMask);
        const uint64_t moveMask = slidingAttackMaskFor(type, pos, blockerMask);
        if (!filled[index]) {
            movesOut[index] = moveMask;
            filled[index] = true;
        }
        else if (filled[index] && movesOut[index] != moveMask) {
            return false;
        }
    }
    return true;
}
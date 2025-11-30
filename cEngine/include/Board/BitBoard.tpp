#pragma once

template<Color color>
void BitBoard::addKingMoves(const int castleRights, std::vector<Move>& moves) const {
    const Pieces::Piece piece = color == Color::White ? Pieces::WHITE_KING : Pieces::BLACK_KING;

    uint64_t kingPos = at(piece);
    const BoardPosition kingStart = positions[pop_lsb(kingPos)];
    if (castleAllowed(color, CastleType::SHORT, castleRights)) {
        constexpr uint64_t shortMask = color == Color::White ? 0x6000000000000000 : 0x60;
        const BoardPosition end = color == Color::White ? BoardPosition{6, 7} : BoardPosition{6, 0};

        if ((shortMask & (at(Color::White) | at(Color::Black))) == 0) {
            moves.push_back(Move::castleMove(kingStart, end, CastleType::SHORT));
        }
    }
    if (castleAllowed(color, CastleType::LONG, castleRights)) {
        constexpr uint64_t longMask = color == Color::White ? 0xe00000000000000 : 0xe;
        const BoardPosition end = color == Color::White ? BoardPosition{2, 7} : BoardPosition{2, 0};

        if ((longMask & (at(Color::White) | at(Color::Black))) == 0) {
            moves.push_back(Move::castleMove(kingStart, end, CastleType::LONG));
        }
    }

    kingPos = at(piece);
    const uint64_t friendly = at(color);
    uint64_t kingMoves = 0;
    kingMoves ^= (kingPos & notA) << 1;

    kingMoves ^= (kingPos & notH) >> 1;

    kingMoves ^= (kingPos & ~rank8) >> 8;
    kingMoves ^= (kingPos & ~rank1) << 8;
    kingMoves ^= (kingPos & (notA & ~rank8)) >> 7;
    kingMoves ^= (kingPos & (notA & ~rank1)) << 9;
    kingMoves ^= (kingPos & (notH & ~rank8)) >> 9;
    kingMoves ^= (kingPos & (notH & ~rank1)) << 7;

    kingMoves &= ~friendly;

    while (kingMoves) {
        moves.push_back(Move::standardMove(kingStart, positions[pop_lsb(kingMoves)]));
    }
}

template<Color color, PieceType type>
void BitBoard::addSlidingMoves(std::vector<Move>& moves) const {
    uint64_t pieces = at(Pieces::make_piece(color, type));
    while (pieces) {
        const int start = pop_lsb(pieces);

        uint64_t realMoves;
        if (type == PieceType::Queen) {
            realMoves = getRealMoves<color, PieceType::Rook>(start);
            realMoves |= getRealMoves<color, PieceType::Bishop>(start);
        } else {
            realMoves = getRealMoves<color, type>(start);
        }

        while (realMoves) {
            const int end = pop_lsb(realMoves);
            moves.push_back(Move::standardMove(positions[start], positions[end]));
        }
    }
}

template<Color color>
void BitBoard::addKnightMoves(std::vector<Move>& moves) const {
    const Pieces::Piece piece = color == Color::White ? Pieces::WHITE_KNIGHT : Pieces::BLACK_KNIGHT;
    const uint64_t friendly = at(color);

    uint64_t knights = at(piece);
    while (knights) {
        const int start = pop_lsb(knights);
        uint64_t possibleMoves = knightMoves[start];
        possibleMoves &= ~friendly;
        while (possibleMoves) {
            const int end = pop_lsb(possibleMoves);
            moves.push_back(Move::standardMove(positions[start], positions[end]));
        }
    }
}

template<Color color>
void BitBoard::addPawnMoves(const std::optional<BoardPosition> enPassantSquare, std::vector<Move>& moves) const {
    using namespace Pieces;
    static constexpr std::array<Piece, 4> whitePieces = {WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN};
    static constexpr std::array<Piece, 4> blackPieces = {BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN};
    const auto& usedPieces = (color == Color::White ? whitePieces : blackPieces);

    static constexpr uint64_t rank7 = 0x000000000000FF00ULL;
    static constexpr uint64_t rank2 = 0x00FF000000000000ULL;

    const uint64_t leftAttacks = color == Color::White ? notH : notA;
    const uint64_t rightAttacks = color == Color::White ? notA : notH;
    const uint64_t enPassantMask = enPassantSquare.has_value() ? 1ULL << shiftValue(enPassantSquare.value()) : 0ULL;
    const uint64_t pawnBoard = at(color == Color::White ? WHITE_PAWN : BLACK_PAWN);
    const uint64_t enemyBoard = color == Color::White ? at(Color::Black) : at(Color::White);
    const uint64_t empty = ~(at(Color::Black) | at(Color::White));
    const uint64_t lastRank = color == Color::White ? rank8 : rank1;
    const uint64_t doubleMoveRank = color == Color::White ? rank2 : rank7;

    const uint64_t combinedForwardMoves = shift<color>(pawnBoard, 8) & empty;
    const uint64_t combinedLeftCaptures = shift<color>(pawnBoard, 7) & leftAttacks & enemyBoard;
    const uint64_t combinedRightCaptures = shift<color>(pawnBoard, 9) & rightAttacks & enemyBoard;


    uint64_t singles = combinedForwardMoves & ~lastRank;
    uint64_t doubles = shift<color>(singles, 8) & empty & shift<color>(doubleMoveRank, 16);
    uint64_t capLeft = combinedLeftCaptures & ~lastRank;
    uint64_t capRight = combinedRightCaptures & ~lastRank;

    uint64_t promotion = combinedForwardMoves & lastRank;
    uint64_t promotionCapLeft = combinedLeftCaptures & lastRank;
    uint64_t promotionCapRight = combinedRightCaptures & lastRank;

    uint64_t enPassantLeft = shift<color>(pawnBoard, 7) & leftAttacks & enPassantMask;
    uint64_t enPassantRight = shift<color>(pawnBoard, 9) & rightAttacks & enPassantMask;

    auto emit = [&](uint64_t bb, const int offset, const int type=1) {
        while (bb) {
            const int to = pop_lsb(bb);
            const int from = to + offset;

            BoardPosition start = positions[from];
            BoardPosition end = positions[to];

            if (type == 2) {
                moves.push_back(Move::enPassantCaptureMove(start, end));
            }
            else if (type == 3) {
                const int enPassantSquareY = end.y - (color == Color::White ? -1 : 1);
                moves.push_back(Move::doublePawnMove(start, end, {end.x, enPassantSquareY}));
            }
            else {
                moves.push_back(Move::standardMove(start,end));
            }
        }
    };

    auto emitPromotion = [&](uint64_t bb, const int offset) {
        while (bb) {
            const int to = pop_lsb(bb);
            const int from = to + offset;
            for (const Piece promoteTo : usedPieces) {
                moves.push_back(Move::promotionMove(positions[from], positions[to], promoteTo));
            }
        }
    };

    emit(singles, color == Color::White ? 8 : -8);
    emit(doubles, color == Color::White ? 16 : -16, 3);
    emit(capLeft, color == Color::White ? 7 : -7);
    emit(capRight, color == Color::White ? 9 : -9);
    emit(enPassantLeft, color == Color::White ? 7 : -7, 2);
    emit(enPassantRight, color == Color::White ? 9 : -9, 2);
    emitPromotion(promotion, color == Color::White ? 8 : -8);
    emitPromotion(promotionCapLeft, color == Color::White ? 7 : -7);
    emitPromotion(promotionCapRight, color == Color::White ? 9 : -9);
}

template<Color color, PieceType type>
uint64_t BitBoard::getRealMoves(const int start) const {
    const Magics* magics = nullptr;
    if (type == PieceType::Rook) {
        magics = &rookMagics;
    } else if (type == PieceType::Bishop) {
        magics = &bishopMagics;
    } else {
        throw std::invalid_argument("Invalid piece type");
    }
    const uint64_t friendly = at(color);
    const uint64_t enemy = at(color == Color::White ? Color::Black : Color::White);
    const uint64_t occupancyKey = (*magics)[start].keyMask & (friendly | enemy);
    uint64_t realMoves = (*magics)[start].getMovesFor(occupancyKey);
    return realMoves & ~friendly;
}
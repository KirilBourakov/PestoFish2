#pragma once

template<Color color>
bool BitBoard::inCheck() const {
    uint64_t attackMask;
    if (color == Color::White) {
        attackMask = getAttackMask<Color::Black>();
    } else {
        attackMask = getAttackMask<Color::White>();
    }
    uint64_t king = at(color == Color::White ? Pieces::WHITE_KING : Pieces::BLACK_KING);
    return (king & attackMask) != 0;
}

template<Color color>
bool BitBoard::isLegalMove(const Move& mv, const Pieces::Piece startContent, const Pieces::Piece endContent) {
    constexpr uint64_t shortMask = color == Color::White ? shortCastleWhite : shortCastleBlack;
    constexpr uint64_t longMask = color == Color::White ? 0xc00000000000000 : 0xc;

    const uint64_t preMove = at(color == Color::White ? Pieces::WHITE_KING : Pieces::BLACK_KING);
    move(mv, startContent, endContent);
    uint64_t attackMask;
    if (color == Color::White) {
        attackMask = getAttackMask<Color::Black>();
    } else {
        attackMask = getAttackMask<Color::White>();
    }

    uint64_t king = at(color == Color::White ? Pieces::WHITE_KING : Pieces::BLACK_KING);
    if (mv.castle.has_value() && mv.castle.value() == CastleType::SHORT) {
        king |= shortMask | preMove; // prevent castling out of check
    }
    if (mv.castle.has_value() && mv.castle.value() == CastleType::LONG) {
        king |= longMask | preMove;
    }

    undoMove(mv, startContent, endContent, color);

    return (king & attackMask) == 0;
}


template<Color color>
uint64_t BitBoard::getKingAttackMask() const {
    const Pieces::Piece piece = color == Color::White ? Pieces::WHITE_KING : Pieces::BLACK_KING;

    uint64_t kingPos = at(piece);

    uint64_t kingMoves = 0;
    kingMoves ^= (kingPos & notA) << 1;

    kingMoves ^= (kingPos & notH) >> 1;

    kingMoves ^= (kingPos & ~rank8) >> 8;
    kingMoves ^= (kingPos & ~rank1) << 8;
    kingMoves ^= (kingPos & (notA & ~rank8)) >> 7;
    kingMoves ^= (kingPos & (notA & ~rank1)) << 9;
    kingMoves ^= (kingPos & (notH & ~rank8)) >> 9;
    kingMoves ^= (kingPos & (notH & ~rank1)) << 7;

    return kingMoves;
}

template<Color color, bool quiescence>
void BitBoard::addKingMoves(const int castleRights, std::vector<Move>& moves) const {
    const Pieces::Piece piece = color == Color::White ? Pieces::WHITE_KING : Pieces::BLACK_KING;

    uint64_t kingPos = at(piece);
    const BoardPosition kingStart = positions[pop_lsb(kingPos)];
    if (!quiescence && castleAllowed(color, CastleType::SHORT, castleRights)) {
        constexpr uint64_t shortMask = color == Color::White ? shortCastleWhite : shortCastleBlack;
        const BoardPosition end = color == Color::White ? BoardPosition{6, 7} : BoardPosition{6, 0};

        if ((shortMask & (at(Color::White) | at(Color::Black))) == 0) {
            moves.push_back(Move::castleMove(kingStart, end, CastleType::SHORT));
        }
    }
    if (!quiescence && castleAllowed(color, CastleType::LONG, castleRights)) {
        constexpr uint64_t longMask = color == Color::White ? longCastleWhite : longCastleBlack;
        const BoardPosition end = color == Color::White ? BoardPosition{2, 7} : BoardPosition{2, 0};

        if ((longMask & (at(Color::White) | at(Color::Black))) == 0) {
            moves.push_back(Move::castleMove(kingStart, end, CastleType::LONG));
        }
    }
    const uint64_t friendly = at(color);
    uint64_t kingMoves = getKingAttackMask<color>() & ~friendly;
    if (quiescence) {
        const uint64_t enemy = at(color == Color::White ? Color::Black : Color::White);
        kingMoves &= enemy;
    }
    while (kingMoves) {
        moves.push_back(Move::standardMove(kingStart, positions[pop_lsb(kingMoves)]));
    }
}

template<Color color, PieceType type>
uint64_t BitBoard::getSlidingAttackMask() const {
    uint64_t pieces = at(Pieces::make_piece(color, type));
    uint64_t out = 0;
    while (pieces) {
        const int start = pop_lsb(pieces);
        uint64_t realMoves;
        if (type == PieceType::Queen) {
            realMoves = getRealMoves<color, PieceType::Rook>(start);
            realMoves |= getRealMoves<color, PieceType::Bishop>(start);
        } else {
            realMoves = getRealMoves<color, type>(start);
        }
        out |= realMoves;
    }
    return out;
}

template<Color color, PieceType type, bool quiescence>
void BitBoard::addSlidingMoves(std::vector<Move>& moves) const {
    uint64_t pieces = at(Pieces::make_piece(color, type));
    const uint64_t friendly = at(color);
    while (pieces) {
        const int start = pop_lsb(pieces);

        uint64_t realMoves;
        if (type == PieceType::Queen) {
            realMoves = getRealMoves<color, PieceType::Rook>(start);
            realMoves |= getRealMoves<color, PieceType::Bishop>(start);
        } else {
            realMoves = getRealMoves<color, type>(start);
        }
        realMoves &= ~friendly;
        if (quiescence) {
            const uint64_t enemy = at(color == Color::White ? Color::Black : Color::White);
            realMoves &= enemy;
        }

        while (realMoves) {
            const int end = pop_lsb(realMoves);
            moves.push_back(Move::standardMove(positions[start], positions[end]));
        }
    }
}

template<Color color>
uint64_t BitBoard::getKnightAttackMask() const {
    const Pieces::Piece piece = color == Color::White ? Pieces::WHITE_KNIGHT : Pieces::BLACK_KNIGHT;

    uint64_t out = 0;
    uint64_t knights = at(piece);
    while (knights) {
        const int start = pop_lsb(knights);
        out |= knightMoves[start];
    }
    return out;
}

template<Color color, bool quiescence>
void BitBoard::addKnightMoves(std::vector<Move>& moves) const {
    const Pieces::Piece piece = color == Color::White ? Pieces::WHITE_KNIGHT : Pieces::BLACK_KNIGHT;
    const uint64_t friendly = at(color);

    uint64_t knights = at(piece);
    while (knights) {
        const int start = pop_lsb(knights);
        uint64_t possibleMoves = knightMoves[start];
        possibleMoves &= ~friendly;
        if (quiescence) {
            const uint64_t enemy = at(color == Color::White ? Color::Black : Color::White);
            possibleMoves &= enemy;
        }
        while (possibleMoves) {
            const int end = pop_lsb(possibleMoves);
            moves.push_back(Move::standardMove(positions[start], positions[end]));
        }
    }
}

template<Color color>
uint64_t BitBoard::getPawnAttackMask() const {
    const uint64_t pawnBoard = at(color == Color::White ? Pieces::WHITE_PAWN : Pieces::BLACK_PAWN);
    const uint64_t leftAttacks = color == Color::White ? notH : notA;
    const uint64_t rightAttacks = color == Color::White ? notA : notH;

    const uint64_t leftCaptures = shift<color>(pawnBoard, 7) & leftAttacks;
    const uint64_t rightCaptures = shift<color>(pawnBoard, 9) & rightAttacks;
    return leftCaptures | rightCaptures;
}

template<Color color, bool quiescence>
void BitBoard::addPawnMoves(const std::optional<BoardPosition> enPassantSquare, std::vector<Move>& moves) const {
    using namespace Pieces;
    static constexpr std::array<Piece, 4> whitePieces = {WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN};
    static constexpr std::array<Piece, 4> blackPieces = {BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN};
    const auto& usedPieces = (color == Color::White ? whitePieces : blackPieces);
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
    uint64_t capLeft = combinedLeftCaptures & ~lastRank;
    uint64_t capRight = combinedRightCaptures & ~lastRank;

    uint64_t promotion = combinedForwardMoves & lastRank;
    uint64_t promotionCapLeft = combinedLeftCaptures & lastRank;
    uint64_t promotionCapRight = combinedRightCaptures & lastRank;

    uint64_t enPassantLeft = shift<color>(pawnBoard, 7) & leftAttacks & enPassantMask;
    uint64_t enPassantRight = shift<color>(pawnBoard, 9) & rightAttacks & enPassantMask;

    emit(capLeft, color == Color::White ? 7 : -7);
    emit(capRight, color == Color::White ? 9 : -9);
    emit(enPassantLeft, color == Color::White ? 7 : -7, 2);
    emit(enPassantRight, color == Color::White ? 9 : -9, 2);
    emitPromotion(promotion, color == Color::White ? 8 : -8);
    emitPromotion(promotionCapLeft, color == Color::White ? 7 : -7);
    emitPromotion(promotionCapRight, color == Color::White ? 9 : -9);

    if (!quiescence) {
        uint64_t singles = combinedForwardMoves & ~lastRank;
        uint64_t doubles = shift<color>(singles, 8) & empty & shift<color>(doubleMoveRank, 16);

        emit(singles, color == Color::White ? 8 : -8);
        emit(doubles, color == Color::White ? 16 : -16, 3);
    }
}

template<Color color, PieceType type>
uint64_t BitBoard::getRealMoves(const int start) const {
    const uint64_t friendly = at(color);
    const uint64_t enemy = at(color == Color::White ? Color::Black : Color::White);
    uint64_t occupancyKey = (friendly | enemy);
    if (type == PieceType::Rook) {
        return rookMagics.getMoves(start, occupancyKey);
    } else if (type == PieceType::Bishop) {
        return bishopMagics.getMoves(start, occupancyKey);
    } else {
        throw std::invalid_argument("Invalid piece type");
    }
}

template<Color color>
uint64_t BitBoard::getAttackMask() const {
    uint64_t moves = 0;
    moves |= getKingAttackMask<color>();
    moves |= getPawnAttackMask<color>();
    moves |= getKnightAttackMask<color>();
    moves |= getSlidingAttackMask<color, PieceType::Bishop>();
    moves |= getSlidingAttackMask<color, PieceType::Rook>();
    moves |= getSlidingAttackMask<color, PieceType::Queen>();
    return moves;
}
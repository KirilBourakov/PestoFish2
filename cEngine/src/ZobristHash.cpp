//
// Created by Kiril on 2025-10-26.
//

module ZobristHash;
import State;
import <random>;
import <array>;

unsigned long long ZobristHash::seed = 90827521673ULL;

ZobristHash::ZobristHash(const State& state):
rng(seed),
dis(
    std::numeric_limits<unsigned long long>::min(),
    std::numeric_limits<unsigned long long>::max()
)
{
    using namespace Pieces;

    static std::array<Piece, 12> pieces{
        WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING,
        BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING
    };
    for (Piece piece : pieces) {
        std::array<std::array<u64, BOARD_SIZE>, BOARD_SIZE> values;
        for (auto i = 0; i < BOARD_SIZE; i++) {
            for (auto j = 0; j < BOARD_SIZE; j++) {
                values[i][j] = dis(rng);
            }
        }
        pieceTable[piece] = values;
    }
    blackToMove = dis(rng);
    for (auto i = 0; i < castleRights.size(); i++) {
        castleRights[i] = dis(rng);
    }
    for (auto i = 0; i < enPassantSquare.size(); i++) {
        enPassantSquare[i] = dis(rng);
    }

    recalculate(state);
}

void ZobristHash::recalculate(const State& state) {
    value = 0;
    for (int i = 0; i < state.getBoard().size(); i++) {
        for (int j = 0; j < state.getBoard()[i].size(); j++) {
            if (Pieces::Piece piece = state.getBoard()[i][j]; piece != Pieces::EMPTY){
                value ^= pieceTable[piece][i][j];
            }
        }
    }
    if (state.getActiveColor() == Color::Black) {
        value ^= blackToMove;
    }
    value ^= castleRights.at(state.getCastlingRights());
    if (state.getEnPassantSquare().has_value()) {
        value ^= enPassantSquare.at(state.getEnPassantSquare().value().x);
    }
}

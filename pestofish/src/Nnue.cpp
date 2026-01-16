//
// Created by Kiril on 1/3/2026.
//

#include "Nnue.hpp"

int Nnue::setBoard(const Board &board, const Color activeColor) const {
    for (int i = 0; i < Weights::HIDDEN_LAYER_SIZE; i++) {
        (*whiteAccumulator)[i] = getWeights()->accumulator_biases[i];
        (*blackAccumulator)[i] = getWeights()->accumulator_biases[i];
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            const Pieces::Piece piece = board.at(y, x);
            if (piece != Pieces::EMPTY) {
                add(calcSquare(y,x), piece);
            }
        }
    }

    return eval(activeColor);
}

void Nnue::move(const Move& mv, const Pieces::Piece startContent, const Pieces::Piece endContent) {
    // remove piece
    if (endContent != Pieces::EMPTY) {
        remove(mv.end, endContent);
    }

    // move
    remove(mv.start, startContent);
    add(mv.end, mv.promotedTo.value_or(startContent));

    // remove pawn behind, if en passant
    if (mv.enPassantCapture) {
        const Pieces::Piece captured = startContent == Pieces::WHITE_PAWN ? Pieces::BLACK_PAWN : Pieces::WHITE_PAWN;
        remove({.x=mv.end.x, .y=mv.start.y}, captured);
    }

    // move rook when castling
    else if (mv.castle == CastleType::LONG) {
        const Pieces::Piece rook = startContent == Pieces::WHITE_KING ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
        remove({.x=0, .y=mv.start.y}, rook);
        add({.x=mv.end.x+1, .y=mv.start.y}, rook);
    }

    else if (mv.castle == CastleType::SHORT) {
        const Pieces::Piece rook = startContent == Pieces::WHITE_KING ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
        remove({.x=7, .y=mv.start.y}, rook);
        add({.x=mv.end.x-1, .y=mv.start.y}, rook);
    }
}

void Nnue::undoMove(const Move& mv, const Pieces::Piece movedPiece, const Pieces::Piece overwrittenPiece, const Color activeColor) {
    if (mv.enPassantCapture) {
        add({.x=mv.end.x, .y=mv.start.y}, (activeColor == Color::White) ? Pieces::BLACK_PAWN : Pieces::WHITE_PAWN);
    }
    else if (mv.castle == CastleType::LONG) {
        const Pieces::Piece rook = (activeColor == Color::White) ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
        remove({.x=mv.end.x+1, .y=mv.start.y}, rook);
        add({.x=0, .y=mv.start.y}, rook);
    }
    else if (mv.castle == CastleType::SHORT) {
        const Pieces::Piece rook = (activeColor == Color::White) ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
        remove({.x=mv.end.x-1, .y=mv.start.y}, rook);
        add({.x=7, .y=mv.start.y}, rook);
    }

    remove(mv.end, mv.promotedTo.value_or(movedPiece));
    add(mv.start, movedPiece);

    if (overwrittenPiece != Pieces::EMPTY) {
        add(mv.end, overwrittenPiece);
    }
}

void Nnue::add(const int square, const Pieces::Piece piece) const {
    int feature = calculateIndex(Color::White, square, piece);
    for (int i = 0; i < whiteAccumulator->size(); i++) {
        (*whiteAccumulator)[i] += getWeights()->accumulator_weights[feature*Weights::HIDDEN_LAYER_SIZE+i];
    }

    feature = calculateIndex(Color::Black, square, piece);
    for (int i = 0; i < blackAccumulator->size(); i++) {
        (*blackAccumulator)[i] += getWeights()->accumulator_weights[feature*Weights::HIDDEN_LAYER_SIZE+i];
    }
}

void Nnue::remove(const int square, const Pieces::Piece piece) const {
    int feature = calculateIndex(Color::White, square, piece);
    for (int i = 0; i < whiteAccumulator->size(); i++) {
        (*whiteAccumulator)[i] -= getWeights()->accumulator_weights[feature*Weights::HIDDEN_LAYER_SIZE + i];
    }

    feature = calculateIndex(Color::Black, square, piece);
    for (int i = 0; i < blackAccumulator->size(); i++) {
        (*blackAccumulator)[i] -= getWeights()->accumulator_weights[feature*Weights::HIDDEN_LAYER_SIZE+i];
    }
}

int Nnue::eval(const Color activeColor) const {
    const auto& first = activeColor == Color::White ? whiteAccumulator : blackAccumulator;
    const auto& second = activeColor == Color::White ? blackAccumulator : whiteAccumulator;

    int sum = 0;
    for (int i = 0; i < Weights::HIDDEN_LAYER_SIZE; ++i) {
        short val = std::clamp((*first)[i], static_cast<short>(0), Weights::QA);
        sum += val * val * getWeights()->output_weights[i];

        val = std::clamp((*second)[i], static_cast<short>(0), Weights::QA);
        sum += val * val * getWeights()->output_weights[i+Weights::HIDDEN_LAYER_SIZE];;
    }

    int output = sum / Weights::QA;
    output += getWeights()->output_bias;
    const int scaled = (output * Weights::SCALE) / (Weights::QA * Weights::QB);
    return activeColor == Color::White ? scaled : -scaled;
}

int Nnue::calculateIndex(const Color perspective, int square, const Pieces::Piece piece) {
    Color color = Pieces::piece_color(piece);

    if (perspective == Color::Black){
        color = color == Color::White ? Color::Black : Color::White;
        square = square ^ 0b111000;
    }

    const int side_offset = color == Color::White ? 0 : 1;
    const int piece_type_offset = static_cast<int>(Pieces::piece_type(piece)) - 1;
    if (piece_type_offset == -1) {
        throw std::logic_error("Running calculateIndex on empty square.");
    }

    return side_offset * 64 * 6 + piece_type_offset * 64 + square;
}
//
// Created by Kiril on 1/3/2026.
//

#include "Nnue.hpp"

int Nnue::setBoard(const Board &board, const Color activeColor) const {
    for (int i = 0; i < L1_OUT; i++) {
        (*whiteAccumulator)[i] = weights->accumulator_biases[i];
        (*blackAccumulator)[i] = weights->accumulator_biases[i];
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            const Pieces::Piece piece = board.at(y, x);
            if (piece != Pieces::EMPTY) {
                const int square = y*8+x;
                add(square, piece);
            }
        }
    }

    return eval(activeColor);
}

void Nnue::add(const int square, const Pieces::Piece piece) const {
    int feature = calculateIndex(Color::White, square, piece);
    for (int i = 0; i < L1_OUT; i++) {
        (*whiteAccumulator)[i] += weights->accumulator_weights[feature][i];
    }

    feature = calculateIndex(Color::Black, square, piece);
    for (int i = 0; i < L1_OUT; i++) {
        (*blackAccumulator)[i] += weights->accumulator_weights[feature][i];
    }
}

void Nnue::remove(const int square, const Pieces::Piece piece) const {
    int feature = calculateIndex(Color::White, square, piece);
    for (int i = 0; i < L1_OUT; i++) {
        (*whiteAccumulator)[i] -= weights->accumulator_weights[feature][i];
    }

    feature = calculateIndex(Color::Black, square, piece);
    for (int i = 0; i < L1_OUT; i++) {
        (*blackAccumulator)[i] -= weights->accumulator_weights[feature][i];
    }
}

int Nnue::eval(const Color activeColor) const {
    const auto& first = activeColor == Color::White ? whiteAccumulator : blackAccumulator;
    const auto& second = activeColor == Color::White ? blackAccumulator : whiteAccumulator;

    int out = weights->output_bias;
    for (int i = 0; i < L1_OUT*2; i++) {
        const int idx = i % L1_OUT;
        const auto& ref = i < L1_OUT ? first : second;
        const int clamped_value = std::clamp((*ref)[idx], 0, static_cast<int32_t>(L1_SCALE));
        out += clamped_value * weights->output_weights[i];
    }

    constexpr double FINAL_DIVISOR = static_cast<double>(L1_SCALE * L2_SCALE) / ((600.0 / 361.0) * 410.0);
    return out / std::round(FINAL_DIVISOR);
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
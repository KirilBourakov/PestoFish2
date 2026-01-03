//
// Created by Kiril on 1/3/2026.
//

#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>

#include "Board/Board.hpp"


constexpr int NNUE_INPUTS = 6 * 2 * 64;
constexpr int L1_OUT = 512;
constexpr std::string WEIGHT_FILE = "weights.json";
constexpr int L1_SCALE = 255;
constexpr int L2_SCALE = 64;
constexpr int SCALE_FACTOR = (600.0 / 361.0) * 410.0;

struct Weights {
    std::array<std::array<int16_t, L1_OUT>, NNUE_INPUTS> accumulator_weights;
    std::array<int16_t, L1_OUT> accumulator_biases;
    std::array<int16_t, L1_OUT*2> output_weights;
    int32_t output_bias;

    template<class Archive>
    void serialize(Archive& ar)
    {
        ar(accumulator_weights, accumulator_biases, output_weights, output_bias);
    }

    static Weights load() {
        Weights weights{};
        if (std::filesystem::exists(WEIGHT_FILE)) {
            std::ifstream is(WEIGHT_FILE);
            cereal::JSONInputArchive archive(is);
            archive(weights);
        } else {
            throw std::logic_error("WEIGHT_FILE not found");
        }
        return weights;
    }
};

class Nnue {
public:
    explicit Nnue() : weights(Weights::load()) {}

    int setBoard(const Board &board, const Color activeColor) {
        whiteAccumulator = weights.accumulator_biases;
        blackAccumulator = weights.accumulator_biases;

        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                const Pieces::Piece piece = board.at(y, x);
                if (piece != Pieces::EMPTY) {
                    const int square = y*8+x;


                    int feature = calculateIndex(Color::White, square, piece);
                    for (int i = 0; i < L1_OUT; i++) {
                        whiteAccumulator[i] += weights.accumulator_weights[feature][i];
                    }

                    feature = calculateIndex(Color::Black, square, piece);
                    for (int i = 0; i < L1_OUT; i++) {
                        blackAccumulator[i] += weights.accumulator_weights[feature][i];
                    }
                }
            }
        }

        const auto& first = activeColor == Color::White ? whiteAccumulator : blackAccumulator;
        const auto& second = activeColor == Color::White ? blackAccumulator : whiteAccumulator;

        int out = weights.output_bias;
        for (int i = 0; i < L1_OUT*2; i++) {
            const int idx = i % L1_OUT;
            const auto& ref = i < L1_OUT ? first : second;
            const int clamped_value = std::clamp(ref[idx], static_cast<int16_t>(0), static_cast<int16_t>(L1_SCALE));
            out += clamped_value * weights.output_weights[i];
        }

        constexpr int FINAL_DIVISOR = std::round(static_cast<double>(L1_SCALE * L2_SCALE) / ((600.0 / 361.0) * 410.0));
        return out / FINAL_DIVISOR;
    }

    static int calculateIndex(const Color perspective, int square, const Pieces::Piece piece){
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

private:
    std::array<int16_t, L1_OUT> whiteAccumulator{};
    std::array<int16_t, L1_OUT> blackAccumulator{};
    Weights weights;
};
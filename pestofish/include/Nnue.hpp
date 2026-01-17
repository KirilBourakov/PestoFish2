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
#include "Resources/model.h"

namespace Weights {
    constexpr int SCALE = 400;
    constexpr short QA = 255;
    constexpr short QB = 64;
    constexpr int INPUT_SIZE = 768;
    constexpr int HIDDEN_LAYER_SIZE = 640;

    constexpr char WEIGHT_FILE[] = "model.nnue";

    struct Values {
        std::array<int16_t, INPUT_SIZE*HIDDEN_LAYER_SIZE> accumulator_weights{};
        std::array<int16_t, HIDDEN_LAYER_SIZE> accumulator_biases{};
        std::array<int16_t, HIDDEN_LAYER_SIZE*2> output_weights{};
        int16_t output_bias{};

        Values() {
            size_t expected_size = (accumulator_weights.size() * sizeof(int16_t)) +
                       (accumulator_biases.size() * sizeof(int16_t)) +
                       (output_weights.size() * sizeof(int16_t)) +
                       sizeof(int16_t);

            if (resources_model_nnue_len < expected_size) {
                throw std::runtime_error("Error: Embedded NNUE data is smaller than expected.");
            }

            const unsigned char* data = resources_model_nnue;

            size_t bytes_acc_weights = accumulator_weights.size() * sizeof(int16_t);
            std::memcpy(accumulator_weights.data(), data, bytes_acc_weights);
            data += bytes_acc_weights;

            size_t bytes_acc_biases = accumulator_biases.size() * sizeof(int16_t);
            std::memcpy(accumulator_biases.data(), data, bytes_acc_biases);
            data += bytes_acc_biases;

            size_t bytes_out_weights = output_weights.size() * sizeof(int16_t);
            std::memcpy(output_weights.data(), data, bytes_out_weights);
            data += bytes_out_weights;

            std::memcpy(&output_bias, data, sizeof(int16_t));
        }
    };
}

class Nnue {
public:
    Nnue()
        : whiteAccumulator(std::make_unique<std::array<short, Weights::HIDDEN_LAYER_SIZE>>())
        , blackAccumulator(std::make_unique<std::array<short, Weights::HIDDEN_LAYER_SIZE>>())
    {}

    void syncAccumulator(const Nnue& source) const {
        for (int x = 0; x < Weights::HIDDEN_LAYER_SIZE; ++x) {
            (*whiteAccumulator)[x] = (*source.whiteAccumulator)[x];
            (*blackAccumulator)[x] = (*source.blackAccumulator)[x];
        }
    }

    int setBoard(const Board &board, Color activeColor) const;
    void move(const Move& mv, Pieces::Piece startContent, Pieces::Piece endContent);
    void undoMove(const Move& mv, Pieces::Piece movedPiece, Pieces::Piece overwrittenPiece, Color activeColor);


    void add(const BoardPosition& pos, const Pieces::Piece piece){
        return add(calcSquare(pos), piece);
    }
    void remove(const BoardPosition& pos, const Pieces::Piece piece){
        return remove(calcSquare(pos), piece);
    }

    [[nodiscard]] int eval(Color activeColor) const;
    static int calculateIndex(Color perspective, int square, Pieces::Piece piece);

private:
    std::unique_ptr<std::array<short, Weights::HIDDEN_LAYER_SIZE>> whiteAccumulator;
    std::unique_ptr<std::array<short, Weights::HIDDEN_LAYER_SIZE>> blackAccumulator;

    void add(int square, Pieces::Piece piece) const;
    void remove(int square, Pieces::Piece piece) const;
    static int calcSquare(const BoardPosition& pos) {
        return calcSquare(pos.y, pos.x);
    }
    static int calcSquare(const int y, const int x) {
        return 63 - (y * 8 + x);
    }

    static const std::unique_ptr<Weights::Values>& getWeights() {
        static auto w = std::make_unique<Weights::Values>();
        return w;
    }
};
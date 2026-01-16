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
            if (std::filesystem::exists(WEIGHT_FILE)) {
                std::ifstream is(WEIGHT_FILE, std::ios::binary);
                if (!is.is_open()) {
                    throw std::runtime_error("WEIGHT_FILE COULD NOT BE OPENED: " + std::string(WEIGHT_FILE));
                }

                is.read(
                    reinterpret_cast<char*>(accumulator_weights.data()),
                    accumulator_weights.size() * sizeof(int16_t)
                );
                is.read(
                    reinterpret_cast<char*>(accumulator_biases.data()),
                    accumulator_biases.size() * sizeof(int16_t)
                );
                is.read(reinterpret_cast<char*>(
                    output_weights.data()),
                    output_weights.size() * sizeof(int16_t)
                );
                is.read(
                    reinterpret_cast<char*>(&output_bias),
                    sizeof(int16_t)
                );

                if (!is) {
                    throw std::runtime_error("Error: File ended prematurely or is corrupt.");
                }
            } else {
                throw std::runtime_error("WEIGHT_FILE not found: " + std::string(WEIGHT_FILE));
            }
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
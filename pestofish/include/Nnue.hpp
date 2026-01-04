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
constexpr char WEIGHT_FILE[] = "weights.json";
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
        ar( CEREAL_NVP(accumulator_weights),
        CEREAL_NVP(accumulator_biases),
        CEREAL_NVP(output_weights),
        CEREAL_NVP(output_bias) );
    }

    void load() {
        std::cout << "Loading" << std::endl;
        std::cout << "Empty weights" << std::endl;
        if (std::filesystem::exists(WEIGHT_FILE)) {
            std::ifstream is(WEIGHT_FILE);
            cereal::JSONInputArchive archive(is);
            this->serialize(archive);
        } else {
            std::cout << "WEIGHT_FILE not found." << std::endl;
            throw std::logic_error("WEIGHT_FILE not found.");
        }
        std::cout << "Done Weights" << std::endl;
    }
};

class Nnue {
public:
    Nnue()
        : whiteAccumulator(std::make_unique<std::array<int32_t, L1_OUT>>())
        , blackAccumulator(std::make_unique<std::array<int32_t, L1_OUT>>())
        , weights(std::make_unique<Weights>())
    {
        weights->load();
    }

    int setBoard(const Board &board, Color activeColor) const;

    void add(int square, Pieces::Piece piece) const;
    void add(const BoardPosition& pos, const Pieces::Piece piece){
        return add(pos.y * 8 + pos.x, piece);
    }

    void remove(int square, Pieces::Piece piece) const;
    void remove(const BoardPosition& pos, const Pieces::Piece piece){
        return remove(pos.y * 8 + pos.x, piece);
    }

    [[nodiscard]] int eval(Color activeColor) const;
    static int calculateIndex(Color perspective, int square, Pieces::Piece piece);

private:
    std::unique_ptr<std::array<int32_t, L1_OUT>> whiteAccumulator;
    std::unique_ptr<std::array<int32_t, L1_OUT>> blackAccumulator;
    std::unique_ptr<Weights> weights;
};
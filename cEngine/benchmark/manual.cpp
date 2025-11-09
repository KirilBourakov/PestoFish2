//
// Created by Kiril on 2025-11-09.
//

#include "Engine.hpp"
#include "Move.hpp"
#include "testBoard.hpp"

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    Engine engine{};
    engine.setState(State{smotheredMatePosition(), Color::White, 0b1111, std::nullopt});

    const Move best = engine.getBestMove();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration_ms = end - start;
    std::cout << "Execution time: " << duration_ms.count() << " ms" << std::endl;
    return 0;
}

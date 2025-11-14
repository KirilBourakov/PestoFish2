//
// Created by Kiril on 2025-11-09.
//

#include "Engine.hpp"
#include "Move.hpp"
#include "testBoard.hpp"

int main() {
    std::chrono::duration<double, std::milli> ms{0};

    Engine engine{};
    engine.setState(State{problemMiddleGame(), Color::White, 0b0000, std::nullopt});
    for (int i = 0; i < 5; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        engine.makeEngineMove();
        auto end = std::chrono::high_resolution_clock::now();
        ms += end - start;
    }

    std::cout << "Execution time: " << ms.count() << " ms" << std::endl;
    return 0;
}

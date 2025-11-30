//
// Created by Kiril on 2025-11-09.
//

#include "Engine.hpp"
#include "Evaluator.hpp"
#include "ModuleOnly/Move.hpp"
#include "testBoard.hpp"
#include "ModuleOnly/parse.hpp"

int main() {
    std::string startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    std::string endGame = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    std::string middleGame = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    std::vector <std::string> positions = {startPos, middleGame, endGame, middleGame};
    constexpr int depth = 4;

    std::chrono::duration<double, std::milli> ms{0};

    for (std::string& position : positions) {
        State state = fenToState(position);
        auto start = std::chrono::high_resolution_clock::now();
        Preft(state, depth);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "1 search completed in " << elapsed.count() << "ms" << std::endl;
        ms += elapsed;
    }
    std::cout << "Execution time: " << ms.count() << " ms" << std::endl;

    return 0;
}

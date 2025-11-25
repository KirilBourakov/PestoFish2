//
// Created by Kiril on 2025-11-09.
//

#include "Engine.hpp"
#include "Evaluator.hpp"
#include "ModuleOnly/Move.hpp"
#include "testBoard.hpp"
#include "ModuleOnly/parse.hpp"

int main() {
    Engine engine{};
    engine.setState(fenToState("r3k2r/p1ppqpb1/bn2pnp1/3PN3/4P1Pp/2N2Q2/1p2BP2/R1B1K2R b KQkq g3 0 1"));

    std::vector<Move> out;
    BoardPosition pos = {6, 5};
    engine.getState().getBoard().bitBoards.addPawnMoves<Color::Black>(pos, out);

    for (auto move : out) {
        std::cout << move << std::endl;
    }

    // engine.setState(fenToState(("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1")));
    // std::cout << Evaluator::evaluate(engine.getState()) << std::endl;
    // // engine.makeEngineMove();
    // engine.getState().makeMove(moveFromLongAlgebric("e7e5", engine.getState()));
    // std::cout << Evaluator::evaluate(engine.getState()) << std::endl;

    // std::chrono::duration<double, std::milli> ms{0};
    //
    // Engine engine{};
    // engine.setState(State{problemMiddleGame(), Color::White, 0b0000, std::nullopt});
    // for (int i = 0; i < 1; i++) {
    //     auto start = std::chrono::high_resolution_clock::now();
    //     engine.makeEngineMove();
    //     auto end = std::chrono::high_resolution_clock::now();
    //     ms += end - start;
    // }
    //
    // std::cout << "Execution time: " << ms.count() << " ms" << std::endl;
    // return 0;
}

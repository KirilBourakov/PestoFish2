//
// Created by Kiril on 2025-11-09.
//

#include "Engine.hpp"
#include "Evaluator.hpp"
#include "ModuleOnly/Move.hpp"
#include "testBoard.hpp"
#include "ModuleOnly/parse.hpp"


unsigned long long Preft(State& state, int depth) {
    if (depth == 0)
        return 1ULL;

    unsigned long long nodes = 0;

    std::vector<Move> moves = state.getMoves();
    for (const Move move : moves) {
        state.makeMove(move);
        nodes += Preft(state, depth - 1);
        state.undoMove();
    }

    return nodes;
}

int main() {
    State state = fenToState("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    // std::cout << Preft(state, 1);

    std::vector<Move> out;
    state.getBoard().getBitBoard().addSlidingMoves<Color::White, PieceType::Bishop>(out);
    std::cout << out.size() << std::endl;
    for (const Move& move : out) {
        std::cout << move << std::endl;
    }


    // Engine engine{};
    // engine.setState(fenToState("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));
    //
    //
    // engine.getState().getBoard().getBitBoard().addRookMoves<Color::White>(out);
    //
    // std::cout << out.size() << std::endl;
    //
    // engine.getState().printState();
    // engine.getState().makeMove(out[0]);
    // engine.getState().printState();
    // engine.getState().undoMove();
    // engine.getState().printState();
    //
    // for (auto move : out) {
    //     std::cout << move << std::endl;
    // }

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

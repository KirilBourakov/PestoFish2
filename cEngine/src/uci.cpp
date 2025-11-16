//
// Created by Kiril on 2025-11-15.
//

#include <iostream>
#include <ostream>
#include <sstream>

#include "Engine.hpp"
#include "ModuleOnly/parse.hpp"

class UCI {
public:
    UCI() {
        engine = std::make_unique<Engine>();
    }

    void runCommand(const std::string& fullCommand) {
        std::vector<std::string> tokens = tokenize(fullCommand);
        std::string command = tokens[0];

        if (command == "isready") {
            std::cout << "readyok" << std::endl;
            return;
        }
        if (command == "ucinewgame") {
            engine = std::make_unique<Engine>();
        }
        if (command == "position") {
            runPosition(std::vector(tokens.begin() + 1, tokens.end()));
        }
        if (command == "go") {
            runGo(std::vector(tokens.begin() + 1, tokens.end()));
        }
        if (command == "stop") {
            engine.get()->forceTimeout();
            worker.join();
        }
        if (command == "quit") {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void runGo(std::vector<std::string> args) {
        if (worker.joinable()) {
            engine.get()->forceTimeout();
            worker.join();
        }

        worker = std::thread([this]() {
            const Move best = this->engine->getBestMove();
            std::cout << "bestmove " << longAlgebricFromMove(best) << std::endl;
        });
    }

    void runPosition(std::vector<std::string> args) {
        for (int i = 0; i < args.size(); i++) {
            std::string token = args[i];
            if (token == "startpos") {
                engine.get()->setState(fenToState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
            }
            if (token == "fen") {
                std::string fen;
                int goal = i + 7;
                for (i = i + 1; i < goal; i++) {
                    fen += args[i] + " ";
                }
                engine.get()->setState(fenToState(fen));
            }

            if (token == "moves") {
                for (i = i + 1; i < args.size(); i++) {
                    Move mv = moveFromLongAlgebric(args[i], engine.get()->getState());
                    engine.get()->getState().makeMove(mv);
                }
            }
        }
    }

    std::vector<std::string> tokenize(const std::string& command) {
        std::istringstream iss(command);
        return {std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
    }

private:
    std::unique_ptr<Engine> engine;
    std::thread worker;
};

int main(void) {
    UCI uci;

    std::string command;
    while (std::getline(std::cin, command)) {
        if (command == "uci") {
            std::cout << "id name PestoFish2" << std::endl;
            std::cout << "id author Kiril Bourakov" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        uci.runCommand(command);
        if (command == "quit") {
            return 0;
        }
    }
}

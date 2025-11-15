//
// Created by Kiril on 2025-11-15.
//

#include <iostream>
#include <ostream>
#include <sstream>

#include "Engine.hpp"

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
            std::string option = tokens[1];
            // Straight start position with no moves
            if (option == "startpos") {
                engine.get()->setState(State::fromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
                // Make any provided moves
                for (int i = 2; i < tokens.size(); i++) {
                    throw std::invalid_argument("UCI does not currently support moves after startpos"); // TODO: handle this case
                }
            }
            if (option == "fen") {
                const std::string fen =
                    tokens[2] + " " + tokens[3] + " " + tokens[4] + " " + tokens[5] + " " + tokens[6] + " " + tokens[7];
                engine.get()->setState(State::fromFen(fen)); // fen should be provided, if fen position is set
            }
        }
        if (command == "go") {
            if (worker.joinable()) {
                engine.get()->forceTimeout();
                worker.join();
            }

            engine->forceTimeout();
            worker = std::thread([this]() {
                const Move best = this->engine->getBestMove();
                std::cout << "bestmove " << best << std::endl;
            });
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
            std::cout << "id name PestoFish2\n";
            std::cout << "id author Kiril Bourakov\n";
            std::cout << "uciok" << std::endl;
        }
        uci.runCommand(command);
        if (command == "quit") {
            return 0;
        }
    }
}

//
// Created by Kiril on 2025-11-15.
//

#include <iostream>
#include <ostream>
#include <regex>
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
            engine.get()->stopSearch();
            worker.join();
        }
        if (command == "quit") {
            if (worker.joinable()) {
                engine.get()->stopSearch();
                worker.join();
            }
        }
    }

    void runGo(std::vector<std::string> args) {
        int i = 0;
        SearchRequest request;
        std::unordered_map<std::string, int*> map = {
            {"movetime", &request.movetime}, {"depth", &request.depth}, {"movestogo", &request.movestogo}, {"binc", &request.binc},
            {"winc", &request.winc},         {"btime", &request.btime}, {"wtime", &request.wtime},
        };
        while (i < args.size()) {
            std::string arg = args.at(i);
            if (arg == "ponder" || arg == "nodes" || arg == "mate") {
                throw std::invalid_argument("ponder, nodes and mate not currently supported");
            } else if (arg == "searchmoves") {
                std::regex pattern("[a-h][1-8][a-h][1-8][nbrqNBRQ]?$");
                std::vector<Move> moves;
                i++;
                while (i < args.size() && std::regex_match(args[i], pattern)) {
                    moves.push_back(moveFromLongAlgebric(args[i], engine->getState()));
                    i++;
                }
                request.searchmoves = moves;
                i--;

            } else if (arg == "infinite") {
                request.infinite = true;
            } else {
                auto it = map.find(args[i]);
                if (it != map.end() && i + 1 < args.size()) {
                    *it->second = std::stoi(args[i + 1]);
                    ++i;
                }
            }

            i++;
        }

        if (worker.joinable()) {
            engine.get()->stopSearch();
            worker.join();
        }

        worker = std::thread([this, request]() {
            const Move best = this->engine->getBestMove(request);
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
                i--;
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

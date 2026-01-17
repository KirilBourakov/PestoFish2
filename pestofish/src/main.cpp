#include <iostream>
#include <string>
#include "pestofish/Interface/UCI.hpp"

int main() {
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
    return 0;
}

#include <iostream>
#include <string>
#include "pestofish/Interface/UCI.hpp"

int main() {
    std::string command;
    bool quit = false;
    UCI uci{quit};

    while (std::getline(std::cin, command) && !quit) {
        uci.runCommand(command);
    }
    return 0;
}

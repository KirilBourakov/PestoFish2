#pragma once

#include <memory>
#include <thread>
#include <vector>
#include <string>

#include "pestofish/Search/Engine.hpp"

class UCI {
public:
    explicit UCI(bool& quit) : quit(quit) {};

    void runCommand(const std::string& fullCommand);

private:
    void runGo(std::vector<std::string> args);
    void runPosition(std::vector<std::string> args);
    std::vector<std::string> tokenize(const std::string& command);

    std::unique_ptr<Engine> engine;
    std::thread worker;
    bool& quit;
};

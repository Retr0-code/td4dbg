#include "CommandExamine.hpp"

#include <bitset>
#include <iomanip>
#include <iostream>

void td4::CommandExamine::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {

    if (arguments.empty()) {
        std::cout << this->_debugger->Emulator()->Info() << '\n';
        return;
    }

    const Emulator::RAM &ram{this->_debugger->Emulator()->Ram()};
    std::hash<std::string> argHash;
    for (const std::string &arg : arguments) {
        if (argHash(arg) == argHash("XY"))
            std::cout << std::setw(4) << std::bitset<4>(ram.at(this->_debugger->Emulator()->Info().XY)) << ' ';
        else
            std::cout << std::setw(4) << std::bitset<4>(ram.at(std::strtol(arg.c_str(), nullptr, 10))) << ' ';
    }

    std::cout << std::endl;
}

std::string td4::CommandExamine::Description(void) {
    return "Shows registers' values.";
}

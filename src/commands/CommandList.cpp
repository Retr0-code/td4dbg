#include "CommandList.hpp"

#include <bitset>
#include <iomanip>
#include <iostream>

void td4::CommandList::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {

    if (arguments.empty()) {
        std::cout << *this->_debugger->Emulator() << '\n';
        return;
    }

    const Emulator::RAM &ram{this->_debugger->Emulator()->Ram()};
    std::hash<std::string> argHash;
    if (argHash(arguments[0]) == argHash("all"))
        std::cout << this->_debugger->Emulator()->List(255).str();
    else
        std::cout << this->_debugger->Emulator()->List(std::strtol(arguments[0].c_str(), nullptr, 10)).str();

    std::cout << std::endl;
}

std::string td4::CommandList::Description(void) {
    return "Shows assembly listing.";
}

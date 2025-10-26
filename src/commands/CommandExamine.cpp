#include "CommandExamine.hpp"

#include <iostream>

void td4::CommandExamine::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {
    std::cout << this->_debugger->Emulator()->Info() << '\n';
}

std::string td4::CommandExamine::Description(void) {
    return "Shows registers' values.";
}

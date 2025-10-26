#include "CommandBreak.hpp"

#include <iostream>

void td4::CommandBreak::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {
    if (arguments.empty()) {
        std::cout << "No instructions indices specified\n";
        return;
    }

    for (const std::string &argument : arguments) {
        int breakpointId{std::strtol(argument.c_str(), nullptr, 10)};
        if (breakpointId <= 0) {
            std::cout << "Invalid value \"" << argument << "\"\n";
            return;
        }

        this->_debugger->ToggleBreak(breakpointId);
    }
}

std::string td4::CommandBreak::Description(void) {
    return "Toggles breakpoints at specified indices.";
}

#include "CommandClear.hpp"

void td4::CommandClear::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {
    this->_debugger->ClearBreaks();
}

std::string td4::CommandClear::Description(void) {
    return "Removes all breakpoints.";
}

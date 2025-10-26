#include "CommandQuit.hpp"

void td4::CommandQuit::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {
    *this->_quit = true;
}

std::string td4::CommandQuit::Description(void) {
    return "Quits the debugger.";
}

#include "CommandHelp.hpp"
#include "../ConsoleInterface.hpp"

#include <iostream>

void td4::CommandHelp::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {
    cli.CommandsReference(std::cout);
}

std::string td4::CommandHelp::Description(void) {
    return "Shows commands reference.";
}

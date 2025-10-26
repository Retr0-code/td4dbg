#include "ICommand.hpp"
#include "CommandStep.hpp"

void td4::CommandStep::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {
    int steps{1};
    if (!arguments.empty()) {
        steps = std::strtol(arguments[0].c_str(), nullptr, 10);

        if (steps <= 0)
            steps = 1;
    }

    this->_debugger->Run(steps, false);
}

std::string td4::CommandStep::Description(void) {
    return "Does a step. If a positive integer number was specified, does a specified amount of steps.";
}

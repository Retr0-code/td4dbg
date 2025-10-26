#include "CommandRun.hpp"

void td4::CommandRun::Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) {
    float frequency{0.f};
    if (!arguments.empty())
        frequency = std::strtof(arguments[0].c_str(), nullptr);

    if (frequency < 0.f)
        frequency = -frequency;

    this->_debugger->SetClockFrequency(frequency);
    this->_debugger->Run(0, true);
}

std::string td4::CommandRun::Description(void) {
    return "Runs debugging until a break point was hit.";
}

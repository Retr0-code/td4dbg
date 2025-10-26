#pragma once

#include "ICommand.hpp"

namespace td4 {
    class CommandRun : public ICommand {
    public:
        CommandRun(const DebuggerPtr &debugger) : ICommand(debugger) {  }

        void Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) override;
        
        std::string Description(void) override;
    };
}

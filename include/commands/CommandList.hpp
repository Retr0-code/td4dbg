#pragma once

#include "ICommand.hpp"

namespace td4 {
    class CommandList : public ICommand {
    public:
        CommandList(const DebuggerPtr &debugger) : ICommand(debugger) {  }

        void Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) override;
        
        std::string Description(void) override;
    };
}

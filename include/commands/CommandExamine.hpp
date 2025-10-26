#pragma once

#include "ICommand.hpp"

namespace td4 {
    class CommandExamine : public ICommand {
    public:
        CommandExamine(const DebuggerPtr &debugger) : ICommand(debugger) {  }

        void Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) override;
        
        std::string Description(void) override;
    };
}

#pragma once

#include "ICommand.hpp"

namespace td4 {
    class CommandQuit : public ICommand {
    public:
        CommandQuit(const DebuggerPtr &debugger, bool *quit)
            : ICommand(debugger), _quit(quit) {  }

        void Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) override;
        
        std::string Description(void) override;
    
    private:
        bool *_quit;
    };
}

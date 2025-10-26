#pragma once
#include <memory>
#include <vector>
#include "../Debugger.hpp"

namespace td4 {

    class ConsoleInterface;

    class ICommand {
    public:
        ICommand(const DebuggerPtr &debugger)
            : _debugger(debugger) {  }

        virtual void Execute(const std::vector<std::string> &arguments, const ConsoleInterface &cli) = 0;
        
        virtual std::string Description(void) = 0;
    
    protected:
        DebuggerPtr _debugger;
    };

    using CommandPtr = std::shared_ptr<ICommand>;

}
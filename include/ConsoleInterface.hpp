#pragma once
#include <any>
#include <string>
#include <istream>
#include <unordered_map>
#include "IConsoleVariable.hpp"

namespace td4 {

    class ConsoleInterface {
    public:
        ConsoleInterface(void) = default;

        ConsoleInterface& ReadCommand(const std::string &command, const std::string &value);

        ConsoleInterface& ReadCommand(std::istream &stream);

        ConsoleInterface& RegisterCommand(const std::string &command, const ConVarPtr &variable);

        std::any operator[](const std::string &command) const;

        ConVarPtr& operator()(const std::string &command);
    
    private:
        std::unordered_map<std::string, ConVarPtr> _commands;
    };

}

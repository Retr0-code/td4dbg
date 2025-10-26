#pragma once
#include <string>
#include <istream>
#include <unordered_map>

#include "Debugger.hpp"
#include "commands/ICommand.hpp"

#define CMD_HELP_SPACE_WIDTH 16

namespace td4 {

    class ConsoleInterface {
    public:
        const ConsoleInterface &ExecuteCommand(const std::string &command, const std::vector<std::string> &arguments) const;

        const ConsoleInterface &ExecuteCommand(std::istream &stream) const;

        template <class T, class... F>
        ConsoleInterface &RegisterCommand(const std::string &command, const DebuggerPtr &debugger, F... args) {
            this->_commands.emplace(command, std::make_shared<T>(debugger, std::forward<F>(args)...));
            return *this;
        }

        CommandPtr operator[](const std::string &command);

        std::ostream &CommandsReference(std::ostream &stream) const;

    private:
        std::unordered_map<std::string, CommandPtr> _commands;
    };

    class UnknownCommand : public std::exception {
    public:
        UnknownCommand(const std::string &command);

        const char *what(void) const throw() override;
    
    private:
        std::string _message;
    };
}

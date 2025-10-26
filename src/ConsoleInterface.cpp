#include <regex>
#include <string>
#include <iomanip>
#include "ConsoleInterface.hpp"

namespace td4 {

    const ConsoleInterface &ConsoleInterface::ExecuteCommand(const std::string &command, const std::vector<std::string> &arguments) const {
        try {
            this->_commands.at(command)->Execute(arguments, *this);
        } catch (...) {
            throw UnknownCommand(command);
        }
        return *this;
    }

    const ConsoleInterface &ConsoleInterface::ExecuteCommand(std::istream &stream) const {
        std::string line;
        std::getline(stream >> std::ws, line);

        std::regex tokenizer(R"((\w+))");
        std::sregex_token_iterator token{line.begin(), line.end(), tokenizer};
        std::sregex_token_iterator tokensEnd;
        
        std::vector<std::string> arguments;
        std::string command{*token};

        for (++token; token != tokensEnd; ++token)
            arguments.emplace_back(std::move(*token));

        this->ExecuteCommand(command, arguments);

        return *this;
    }

    CommandPtr ConsoleInterface::operator[](const std::string &command) {
        return this->_commands.at(command);
    }

    std::ostream &ConsoleInterface::CommandsReference(std::ostream &stream) const {
        for (auto [command, commandPtr] : this->_commands) {
            stream << std::left << std::setw(CMD_HELP_SPACE_WIDTH) << command;
            stream << commandPtr->Description() << '\n';
        }
        return stream;
    }

    UnknownCommand::UnknownCommand(const std::string &command) {
        this->_message = "Unkown command \"" + command + '\"';
    }
    
    const char *UnknownCommand::what(void) const throw() {
        return _message.c_str();
    }
}
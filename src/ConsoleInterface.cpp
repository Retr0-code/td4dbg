#include <regex>
#include <string>
#include "ConsoleInterface.hpp"

namespace td4 {
    ConsoleInterface &ConsoleInterface::ReadCommand(const std::string &command, const std::string &value) {
        this->_commands.at(command)->SetValue(value);
        return *this;
    }

    ConsoleInterface &ConsoleInterface::ReadCommand(std::istream &stream) {
        std::string line;
        std::getline(stream, line);

        std::regex pattern(R"(\s*(\S+)\s+(\S+))");
        std::smatch regexTokens;
        
        std::regex_match(line, regexTokens, pattern);

        std::string command{*(regexTokens.begin() + 1)};
        std::string argument{*(regexTokens.begin() + 2)};

        this->ReadCommand(command, argument);

        return *this;
    }

    ConsoleInterface &ConsoleInterface::RegisterCommand(const std::string &command, const ConVarPtr &variable) {
        this->_commands.emplace(command, std::move(variable));
        return *this;
    }

    std::any ConsoleInterface::operator[](const std::string &command) const {
        return this->_commands.at(command)->GetValue();
    }
    
    ConVarPtr &ConsoleInterface::operator()(const std::string &command) {
        return this->_commands.at(command);
    }
}
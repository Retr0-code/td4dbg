#include "ConsoleVariables.hpp"

namespace td4 {

    ConVarFloat::ConVarFloat(void) : _value{0} {  }

    void ConVarFloat::SetValue(const std::any &value) {
        this->_value = std::any_cast<Type>(value);
    }

    void ConVarFloat::SetValue(const std::string &value) {
        this->_value = std::strtof(value.c_str(), nullptr);
    }

    std::any ConVarFloat::GetValue(void) const {
        return std::any(this->_value);
    }

    ConVarInt::ConVarInt(void) : _value{0} {  }

    void ConVarInt::SetValue(const std::any &value) {
        this->_value = std::any_cast<Type>(value);
    }

    void ConVarInt::SetValue(const std::string &value) {
        this->_value = std::strtol(value.c_str(), nullptr, 10);
    }

    std::any ConVarInt::GetValue(void) const {
        return std::any(this->_value);
    }

}

#pragma once
#include "IConsoleVariable.hpp"

namespace td4 {

    class ConVarFloat : public IConsoleVariable {
    public:
        using Type = float;

        ConVarFloat(void);

        void SetValue(const std::any &value) override;

        void SetValue(const std::string &value) override;
        
        std::any GetValue(void) const override;

    private:
        Type _value;
    };

    class ConVarInt : public IConsoleVariable {
    public:
        using Type = int;

        ConVarInt(void);

        void SetValue(const std::any &value) override;

        void SetValue(const std::string &value) override;
        
        std::any GetValue(void) const override;

    private:
        Type _value;
    };

}

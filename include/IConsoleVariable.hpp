#pragma once
#include <any>
#include <memory>

namespace td4 {

    class IConsoleVariable {
    public:
        virtual void SetValue(const std::any &value) = 0;
        
        virtual void SetValue(const std::string &value) = 0;
        
        virtual std::any GetValue(void) const = 0;
    };

    using ConVarPtr = std::shared_ptr<IConsoleVariable>;

}

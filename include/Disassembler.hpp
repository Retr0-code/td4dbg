#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>

namespace td4 {

    class Disassembler {
    public:
        // bool ? binary : unary; operation;
        using OperatorCombination = std::pair<bool, std::string>;
        
        Disassembler(void);

        const OperatorCombination& operator[](uint8_t opcode) const;

    private:
        std::unordered_map<int, OperatorCombination> _decoder;
    };

}

#pragma once
#include <array>
#include <memory>
#include <ostream>
#include <cstdint>
#include <functional>

#include "Disassembler.hpp"

#define PROGRAM_MAX_LEN 256

namespace td4 {

    using Register = uint8_t;

    struct Registers {
        Register A;     // 4-bit register
        Register B;     // 4-bit register
        Register PC;    // 8-bit register
        Register XY;    // 8-bit register
        Register IN;    // 4-bit register
        Register OUT;   // 4-bit register
        Register CF;    // 1-bit register
        Register ZF;    // 1-bit register

        Registers(void);

        friend std::ostream &operator<<(std::ostream &out, const Registers &registers);
    };

    class Emulator {
    public:
        using Program = std::array<uint8_t, PROGRAM_MAX_LEN>;
        using RAM = std::array<uint8_t, PROGRAM_MAX_LEN>;
        using InputMethod = std::function<void (Register&)>;

        Emulator(const Program& program);

        const Registers& Step(void);
        
        const Registers& Info(void) const;

        std::stringstream ListInstruction(uint8_t address) const;

        std::stringstream List(uint8_t maxOutput) const;
        
        const RAM& Ram(void) const;

        void SetInputMethod(const InputMethod& input);

        friend std::ostream &operator<<(std::ostream &out, const Emulator &emulator);

    private:        
        Register Select(void);
        
        uint8_t ALU(Register operandLeft);

        void Load(uint8_t result);

    private:
        Program      _program;
        RAM          _ram;
        InputMethod  _inputMethod;
        Disassembler _disassembler;
        Registers    _registers;
    };

    using EmulatorPtr = std::shared_ptr<Emulator>;
}

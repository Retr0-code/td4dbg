#pragma once
#include <array>
#include <memory>
#include <ostream>
#include <cstdint>
#include <functional>

#include "Disassembler.hpp"

#define PROGRAM_MAX_LEN 16

namespace td4 {

    using Register = uint8_t;

    struct Registers {
        Register A;
        Register B;
        Register CF;
        Register PC;
        Register IN;
        Register OUT;

        Registers(void);

        friend std::ostream &operator<<(std::ostream &out, const Registers &registers);
    };

    class Emulator {
    public:
        using Program = std::array<uint8_t, PROGRAM_MAX_LEN>;
        using InputMethod = std::function<void (Register&)>;

        Emulator(const Program& program);

        const Registers& Step(void);
        
        const Registers& Info(void) const;

        void SetInputMethod(const InputMethod& input);

        friend std::ostream &operator<<(std::ostream &out, const Emulator &emulator);

    private:
        Register Select(void);
        
        uint8_t Add(Register operandLeft);

        void Load(uint8_t result);

    private:
        Program _program;
        InputMethod _inputMethod;
        Disassembler _disassembler;
        Registers _registers;
    };

    using EmulatorPtr = std::shared_ptr<Emulator>;
}

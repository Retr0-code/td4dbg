#include <cstdio>
#include "Emulator.hpp"

namespace td4 {

    Registers::Registers(void)
        : A{0}, B{0}, CF{0}, PC{0}, IN{0}, OUT{0} {  }

    Emulator::Emulator(const Program &program)
        : _program{program} {
            this->SetInputMethod([](Register& in) { std::scanf("%i", &in); });
        }

    const Registers &Emulator::Step(void) {
        // Decode instruction
        this->Load(
            this->Add(
                this->Select()
            )
        );

        ++this->_registers.PC;
        if (this->_registers.PC & 0xf0)
            this->_registers.PC = 0;

        return this->_registers;
    }

    void Emulator::SetInputMethod(const InputMethod &input) {
        this->_inputMethod = input;
    }

    Register Emulator::Select(void) {
        uint8_t opcode{this->_program.at(this->_registers.PC)};

        bool sel_a{opcode & 0x90};  // SEL_A (D4 || D7)
        bool sel_b{opcode & 0x20};  // SEL_B D5

        if (sel_a && sel_b)
            return 0;

        if (sel_a)
            return this->_registers.B;

        if (sel_b) {
            _inputMethod(this->_registers.IN);
            this->_registers.IN &= 0x0f;
            return this->_registers.IN;
        }

        return this->_registers.A;
    }

    uint8_t Emulator::Add(Register operandLeft) {
        uint8_t immediate{this->_program.at(this->_registers.PC) & 0x0f};
        uint8_t result{operandLeft + immediate};
        if (result >> 4)
            this->_registers.CF = 1;

        return (result + (result >> 4)) & 0x0f;
    }

    void Emulator::Load(uint8_t result) {
        uint8_t opcode{this->_program.at(this->_registers.PC)};

        // LOAD3 !((CF || D4) && D67)
        if (!((!this->_registers.CF || opcode & 0x10) && opcode & 0x40 && opcode & 0x80))
            _registers.PC = result;

        // LOAD2 !(D67)
        if (!(opcode & 0x40 && opcode & 0x80))
            this->_registers.OUT = result;

        // LOAD1 !D6 || D7
        if (!(opcode & 0x40) || (opcode & 0x80))
            this->_registers.B = result;

        // LOAD0 D6 || D7
        if (opcode & 0xc0)
            this->_registers.A = result;
    }
}

#include <cstdio>
#include <bitset>
#include <iomanip>
#include "Emulator.hpp"

namespace td4 {

    Registers::Registers(void)
        : A{0}, B{0}, CF{0}, PC{0}, IN{0}, OUT{0} {  }

    std::ostream &operator<<(std::ostream &out, const Registers &registers) {
        out << "A:\t" << std::bitset<4>(registers.A) << '\t'
            << "B:\t" << std::bitset<4>(registers.B)  << '\n';
        out << "CF:\t" << std::bitset<4>(registers.CF) << '\t'
            << "PC:\t" << std::bitset<4>(registers.PC) << '\n';
        out << "IN:\t" << std::bitset<4>(registers.IN) << '\t'
            << "OUT:\t" << std::bitset<4>(registers.OUT) << '\n';
        
        return out;
    }

    Emulator::Emulator(const Program &program)
        : _program{program} {
            this->SetInputMethod([](Register& in) {
                std::puts("INPUT PORT:");
                std::scanf("%i", &in);
            });
        }

    const Registers &Emulator::Step(void) {
        this->_registers.CF = 0;
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

    std::ostream &operator<<(std::ostream &out, const Emulator &emulator) {
        uint8_t address{0};
        for (auto opcode : emulator._program) {
            out << std::bitset<4>(address) << std::setw(4) << std::setfill(' ')
                << (emulator._registers.PC == address ? "=>" : "") << ' '
                << std::bitset<4>(opcode >> 4) << ':' << std::bitset<4>(opcode) << '\t';
                
            Disassembler::OperatorCombination op{emulator._disassembler[opcode & 0xf0]};
            out << op.second;
            if (op.first)
                out << std::bitset<4>(opcode);

            out << std::endl;

            ++address;
        }
        return out;
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
        if (result & 0xf0)
            this->_registers.CF = 1;

        return result & 0x0f;
    }

    void Emulator::Load(uint8_t result) {
        uint8_t opcode{this->_program.at(this->_registers.PC)};

        // LOAD3 ((CF || D4) && D67)
        if (((!this->_registers.CF || opcode & 0x10) && opcode & 0x40 && opcode & 0x80))
            _registers.PC = result;

        // LOAD2 !((!D6)7)
        if ((!(opcode & 0x40) && opcode & 0x80))
            this->_registers.OUT = result;

        // LOAD1 !D6 || D7
        if ((opcode & 0x40) && !(opcode & 0x80))
            this->_registers.B = result;

        // LOAD0 D6 || D7
        if (!(opcode & 0xc0))
            this->_registers.A = result;
    }
}

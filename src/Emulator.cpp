#include <bitset>
#include <iomanip>
#include <iostream>
#include "Emulator.hpp"

namespace td4 {

    Registers::Registers(void)
        : A{0}, B{0}, PC{0}, XY{0}, IN{0}, OUT{0}, CF{0}, ZF{0} {  }

    std::ostream &operator<<(std::ostream &out, const Registers &registers) {
        out << "A:\t" << std::bitset<4>(registers.A) << '\t'
            << "B:\t" << std::bitset<4>(registers.B)  << '\n';

        out << "PC:\t" << std::bitset<8>(registers.PC) << '\t'
            << "XY:\t" << std::bitset<8>(registers.XY) << '\n';
        
        out << "CF:\t" << std::bitset<1>(registers.CF) << '\t'
            << "CF:\t" << std::bitset<1>(registers.ZF) << '\n';
        
        out << "IN:\t" << std::bitset<4>(registers.IN) << '\t'
            << "OUT:\t" << std::bitset<4>(registers.OUT) << '\n';
        
        return out;
    }

    Emulator::Emulator(const Program &program)
        : _program{program} {
            this->SetInputMethod([](Register& in) {
                int tmp = 0;
                std::cout << "INPUT PORT: ";
                std::cin >> tmp;
                in = tmp & 0x0f;
            });
        }

    const Registers &Emulator::Step(void) {
        this->_registers.CF = 0;
        this->_registers.ZF = 0;
        this->Load(
            this->ALU(
                this->Select()
            )
        );

        ++this->_registers.PC;
        return this->_registers;
    }

    const Registers &Emulator::Info(void) const {
        return this->_registers;
    }

    std::stringstream Emulator::ListInstruction(uint8_t address) const {
        std::stringstream sstream;
        uint8_t opcode{this->_program.at(address)};
        sstream << std::bitset<4>(address) << std::setw(4) << std::setfill(' ')
            << (this->_registers.PC == address ? "=>" : "") << ' '
            << std::bitset<4>(opcode >> 4) << ':' << std::bitset<4>(opcode) << '\t';

        Disassembler::OperatorCombination op{this->_disassembler[(opcode & 0xf0) == 0x80 ? opcode : opcode & 0xf0]};
        sstream << op.second;
        if (op.first)
            sstream << std::bitset<4>(opcode);

        sstream << std::endl;

        return sstream;
    }

    std::stringstream Emulator::List(uint8_t maxOutput) const {
        uint8_t address{0};
        std::stringstream sstream;

        for (auto opcode : this->_program) {
            sstream << ListInstruction(address).str();

            if (address == maxOutput)
                break;

            ++address;
        }

        return sstream;
    }

    const Emulator::RAM &Emulator::Ram(void) const {
        return this->_ram;
    }

    void Emulator::SetInputMethod(const InputMethod &input) {
        this->_inputMethod = input;
    }

    std::ostream &operator<<(std::ostream &out, const Emulator &emulator) {
        out << emulator.List(8).str();
        return out;
    }

    Register Emulator::Select(void) {
        uint8_t opcode{this->_program.at(this->_registers.PC)};

        bool sel_a{opcode & 0x90};  // SEL_A (D4 || D7)
        bool sel_b{opcode & 0x20};  // SEL_B D5
        uint8_t lh{opcode & 0x0f};

        if ((opcode & 0xf0) == 0x80) {
            if (lh == 0x01 || lh == 0x02 || lh == 0x07 || lh == 0x09 || lh == 0x0c || lh == 0x0d)
                return this->_registers.A;

            if (lh == 0x03 || lh == 0x04 || lh == 0x05 || lh == 0x06 || lh == 0x0b)
                return this->_registers.B;

            if (lh == 0x0e || lh == 0x0f)
                return this->_registers.XY;

            if (lh == 0x08 || lh == 0x0a)
                return this->_ram[this->_registers.XY];
        }

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

    uint8_t Emulator::ALU(Register operandLeft) {
        uint8_t lh{this->_program.at(this->_registers.PC) & 0x0f};
        uint8_t hh{this->_program.at(this->_registers.PC) & 0xf0};
        uint8_t result{0};

        if (hh == 0x80) {
            switch (lh) {
                case 0x00:
                    result = this->_registers.A + operandLeft;
                    break;

                case 0x01:
                    result = -operandLeft;
                    break;

                case 0x02:
                    result = ~operandLeft;
                    break;

                case 0x04:
                    result = this->_registers.A & operandLeft;
                    break;

                case 0x05:
                    result = this->_registers.A ^ operandLeft;
                    break;

                case 0x06:
                    result = this->_registers.A - operandLeft;
                    break;

                case 0x0e:
                    result = operandLeft + 1;
                    break;

                default:
                    result = operandLeft;
                    break;
            }
        } else
            result = operandLeft + lh;

        if (result & 0xf0)
            this->_registers.CF = 1;

        if (result == 0)
            this->_registers.ZF = 1;

        return result & 0x0f;
    }

    void Emulator::Load(uint8_t result) {
        uint8_t opcode{this->_program.at(this->_registers.PC)};

        // LOAD3 (((!CF || ZF) || D4) && D67)
        if ((((!this->_registers.CF || this->_registers.ZF)
            || opcode & 0x10) && (opcode & 0x40) && (opcode & 0x80)) || opcode == 0x8f)
            _registers.PC = result;

        // LOAD2 !((!D6)D7)
        if (opcode == 0x87 || opcode == 0x90)
            this->_registers.OUT = result;

        // LOAD1 !D6 || D7
        if ((opcode & 0x40) && !(opcode & 0x80) || opcode == 0x8a)
            this->_registers.B = result;

        // LOAD0 D6 || D7
        if (!(opcode & 0xc0) || (opcode >= 0x80 && opcode <= 0x86) || opcode == 0x88)
            this->_registers.A = result;

        if (opcode == 0x8d || (opcode & 0xf0) == 0xc0)
            this->_registers.XY = result << 4;

        if (opcode == 0x8c || opcode == 0x8e || (opcode & 0xf0) == 0xd0)
            this->_registers.XY = result;

        if (opcode == 0x89 || opcode == 0x8b)
            this->_ram[this->_registers.XY] = result;
    }
}

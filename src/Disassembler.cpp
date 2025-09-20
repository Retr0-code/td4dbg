#include "Disassembler.hpp"

#define TD4_MOVAIM 0x30
#define TD4_MOVBIM 0x70
#define TD4_MOVAB  0x10
#define TD4_MOVBA  0x40
#define TD4_ADDAIM 0x00
#define TD4_ADDBIM 0x50
#define TD4_JMPIM  0xf0
#define TD4_JNCIM  0xe0
#define TD4_OUTIM  0xb0
#define TD4_OUTB   0x90
#define TD4_INA    0x20
#define TD4_INB    0x60

td4::Disassembler::Disassembler(void) {
    this->_decoder.emplace(TD4_MOVAIM,  OperatorCombination{1, "MOV A, "});
    this->_decoder.emplace(TD4_MOVBIM,  OperatorCombination{1, "MOV B, "});
    this->_decoder.emplace(TD4_MOVAB,   OperatorCombination{0, "MOV A, B"});
    this->_decoder.emplace(TD4_MOVBA,   OperatorCombination{0, "MOV B, A"});
    this->_decoder.emplace(TD4_ADDAIM,  OperatorCombination{1, "ADD A, "});
    this->_decoder.emplace(TD4_ADDBIM,  OperatorCombination{1, "ADD B, "});
    this->_decoder.emplace(TD4_JMPIM,   OperatorCombination{1, "JMP "});
    this->_decoder.emplace(TD4_JNCIM,   OperatorCombination{1, "JNC "});
    this->_decoder.emplace(TD4_OUTIM,   OperatorCombination{1, "OUT "});
    this->_decoder.emplace(TD4_OUTB,    OperatorCombination{0, "OUT B"});
    this->_decoder.emplace(TD4_INA,     OperatorCombination{0, "IN A"});
    this->_decoder.emplace(TD4_INB,     OperatorCombination{0, "IN B"});
}

const td4::Disassembler::OperatorCombination &td4::Disassembler::operator[](uint8_t opcode) const {
    return this->_decoder.at(opcode);
}

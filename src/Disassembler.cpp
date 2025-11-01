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

#define TD4M_MOVYIM 0xc0
#define TD4M_MOVXIM 0xd0
#define TD4M_JZIM   0xa0
#define TD4M_ADDAB  0x80
#define TD4M_NEGA   0x81
#define TD4M_NOTA   0x82
#define TD4M_ORAB   0x83
#define TD4M_ANDAB  0x84
#define TD4M_XORAB  0x85
#define TD4M_SUBAB  0x86
#define TD4M_OUTA   0x87
#define TD4M_LDA    0x88
#define TD4M_STA    0x89
#define TD4M_LDB    0x8a
#define TD4M_STB    0x8b
#define TD4M_MOVXA  0x8c
#define TD4M_MOVYA  0x8d
#define TD4M_INCXY  0x8e
#define TD4M_JMPXY  0x8f

td4::Disassembler::Disassembler(void) {
    this->_decoder.emplace(TD4_MOVAIM,  OperatorCombination{1, "MOV A, "});
    this->_decoder.emplace(TD4_MOVBIM,  OperatorCombination{1, "MOV B, "});
    this->_decoder.emplace(TD4_MOVAB,   OperatorCombination{1, "MOV A, B, "});
    this->_decoder.emplace(TD4_MOVBA,   OperatorCombination{1, "MOV B, A, "});
    this->_decoder.emplace(TD4_ADDAIM,  OperatorCombination{1, "ADD A, "});
    this->_decoder.emplace(TD4_ADDBIM,  OperatorCombination{1, "ADD B, "});
    this->_decoder.emplace(TD4_JMPIM,   OperatorCombination{1, "JMP "});
    this->_decoder.emplace(TD4_JNCIM,   OperatorCombination{1, "JNC "});
    this->_decoder.emplace(TD4_OUTIM,   OperatorCombination{1, "OUT "});
    this->_decoder.emplace(TD4_OUTB,    OperatorCombination{1, "OUT B, "});
    this->_decoder.emplace(TD4_INA,     OperatorCombination{1, "IN A, "});
    this->_decoder.emplace(TD4_INB,     OperatorCombination{1, "IN B, "});

    this->_decoder.emplace(TD4M_MOVXIM, OperatorCombination{1, "MOV X, "});
    this->_decoder.emplace(TD4M_MOVYIM, OperatorCombination{1, "MOV Y, "});
    this->_decoder.emplace(TD4M_JZIM,   OperatorCombination{1, "JZ "});
    this->_decoder.emplace(TD4M_ADDAB,  OperatorCombination{0, "ADD A, B"});
    this->_decoder.emplace(TD4M_NEGA,   OperatorCombination{0, "NEG A"});
    this->_decoder.emplace(TD4M_NOTA,   OperatorCombination{0, "NOT A"});
    this->_decoder.emplace(TD4M_ORAB,   OperatorCombination{0, "OR A, B"});
    this->_decoder.emplace(TD4M_ANDAB,  OperatorCombination{0, "AND A, B"});
    this->_decoder.emplace(TD4M_XORAB,  OperatorCombination{0, "XOR A, B"});
    this->_decoder.emplace(TD4M_SUBAB,  OperatorCombination{0, "SUB A, B"});
    this->_decoder.emplace(TD4M_OUTA,   OperatorCombination{0, "OUT A"});
    this->_decoder.emplace(TD4M_LDA,    OperatorCombination{0, "LD A"});
    this->_decoder.emplace(TD4M_STA,    OperatorCombination{0, "ST A"});
    this->_decoder.emplace(TD4M_LDB,    OperatorCombination{0, "LD B"});
    this->_decoder.emplace(TD4M_STB,    OperatorCombination{0, "ST B"});
    this->_decoder.emplace(TD4M_MOVXA,  OperatorCombination{0, "MOV X, A"});
    this->_decoder.emplace(TD4M_MOVYA,  OperatorCombination{0, "MOV Y, A"});
    this->_decoder.emplace(TD4M_INCXY,  OperatorCombination{0, "INC XY"});
    this->_decoder.emplace(TD4M_JMPXY,  OperatorCombination{0, "JMP XY"});
}

const td4::Disassembler::OperatorCombination &td4::Disassembler::operator[](uint8_t opcode) const {
    return this->_decoder.at(opcode);
}

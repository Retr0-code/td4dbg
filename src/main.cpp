#include <iomanip>
#include <fstream>
#include <iostream>

#include "Emulator.hpp"

std::ostream &operator<<(std::ostream &out, const td4::Registers &registers);

int main(int argc, const char **argv) {
    if (argc != 2) {
        std::cerr << "Please specify TD4 program file as a positional argument\n";
        return 1;
    }
    
    // Reading input file
    std::ifstream inputFile(argv[1], std::ios::binary | std::ios::ate);
    if (!inputFile.is_open()) {
        std::cerr << "E: Unable to open specified file " << argv[2] << '\n';
        return 1;
    }

    td4::Emulator::Program program;
    size_t fileSize{inputFile.tellg()};
    if (fileSize > program.max_size())
        fileSize = program.max_size();

    inputFile.seekg(std::ios::beg);
    inputFile.read(reinterpret_cast<char*>(program.data()), fileSize);

    // Invoking debugger CLI (interactive command line)

    // Initializing emulator
    td4::Emulator emulator(program);
    for (auto instruction : program) {
        td4::Registers status{emulator.Step()};

        // Pretty print
        std::cout << std::hex << (int)instruction << '\n';
        std::cout << status << '\n';
    }
    
    return 0;
}

std::ostream &operator<<(std::ostream &out, const td4::Registers &registers) {
    out << "A: " << (int)registers.A << '\n';
    out << "B: " << (int)registers.B << '\n';
    out << "CF: " << (int)registers.CF << '\n';
    out << "PC: " << (int)registers.PC << '\n';
    out << "IN: " << (int)registers.IN << '\n';
    out << "OUT: " << (int)registers.OUT << '\n';
    
    return out;
}

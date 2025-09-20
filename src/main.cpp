#include <bitset>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>

#include "Emulator.hpp"
#include "ConsoleInterface.hpp"
#include "ConsoleVariables.hpp"

static td4::ConVarInt::Type programStep{0};

int main(int argc, const char **argv) {
    using namespace std::chrono;

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

    td4::ConsoleInterface cli;
    cli.RegisterCommand("clock", std::make_shared<td4::ConVarFloat>())
       .RegisterCommand("step",  std::make_shared<td4::ConVarInt>())
       .RegisterCommand("run",  std::make_shared<td4::ConVarInt>())
       .RegisterCommand("quit",  std::make_shared<td4::ConVarInt>());

    td4::Emulator emulator(program);
    td4::ConVarFloat::Type frequency;
    td4::ConVarInt::Type quit{0};
    while (true) {
        if (programStep <= 0 || frequency == 0.f) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cli("run")->SetValue(std::any(0));
            int run{std::any_cast<td4::ConVarInt::Type>(cli["run"])};
            while (!run) {
                std::cout << "> " << std::flush;

                try {
                    cli.ReadCommand(std::cin);
                    frequency = std::any_cast<td4::ConVarFloat::Type>(cli["clock"]);
                    programStep = std::any_cast<td4::ConVarInt::Type>(cli["step"]);
                    run = std::any_cast<td4::ConVarInt::Type>(cli["run"]);
                    quit = std::any_cast<td4::ConVarInt::Type>(cli["quit"]);
                    
                    if (quit)
                        break;

                    if (programStep && frequency == 0.f)
                        break;

                } catch (...) {
                    std::cout << "Invalid command" << std::endl;
                }
            }
        }

        if (quit)
            break;
        
        steady_clock::time_point begin = steady_clock::now();
        steady_clock::time_point end = begin;
        while (frequency != 0.f && duration_cast<milliseconds>(end - begin).count() < 1000.f / frequency)
            end = steady_clock::now();

        --programStep;

        td4::Registers status{emulator.Step()};

        std::cout << emulator << std::endl;
        std::cout << status << std::flush;
    }
    
    return 0;
}

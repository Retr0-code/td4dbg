#include <bitset>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>

#include <signal.h>

#include "Emulator.hpp"
#include "ConsoleInterface.hpp"
#include "ConsoleVariables.hpp"

static td4::ConVarPtr programStep = std::make_shared<td4::ConVarInt>();

void StopSimulation(int) {
    programStep->SetValue(std::any{0});
    std::cout << std::endl;
}

#define CastFloat(value) std::any_cast<td4::ConVarFloat::Type>(value)
#define CastInt(value) std::any_cast<td4::ConVarInt::Type>(value)

int main(int argc, const char **argv) {
    signal(SIGINT, &StopSimulation);

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
    program.fill(0);
    inputFile.read(reinterpret_cast<char*>(program.data()), fileSize);

    td4::ConsoleInterface cli;
    td4::ConVarPtr frequency = std::make_shared<td4::ConVarFloat>();
    td4::ConVarPtr run = std::make_shared<td4::ConVarInt>();
    td4::ConVarPtr quit = std::make_shared<td4::ConVarInt>();
    cli.RegisterCommand("clock", frequency)
       .RegisterCommand("step",  programStep)
       .RegisterCommand("run",   run)
       .RegisterCommand("quit",  quit);

    td4::Emulator emulator(program);
    std::cout << "Press ENTER to continue" << std::endl;
    while (true) {
        if (CastInt(**programStep) <= 0 || CastFloat(**frequency) == 0.f) {

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cli("run")->SetValue(std::any(0));
            while (!CastInt(**run)) {
                std::cout << "> " << std::flush;

                try {
                    cli.ReadCommand(std::cin);
                    
                    if (CastInt(**quit))
                        break;

                    if (CastInt(**programStep) && CastFloat(**frequency) == 0.f)
                        break;

                } catch (...) {
                    std::cout << "Invalid command" << std::endl;
                }
            }
        }

        if (CastInt(**quit))
            break;
        
        steady_clock::time_point begin = steady_clock::now();
        steady_clock::time_point end = begin;
        while (CastFloat(**frequency) != 0.f &&
            duration_cast<milliseconds>(end - begin).count() < 1000.f / CastFloat(**frequency))
            
            end = steady_clock::now();

        programStep->SetValue(CastInt(**programStep) - 1);

        td4::Registers status{emulator.Step()};

        std::cout << emulator << std::endl;
        std::cout << status << std::endl;
    }
    
    return 0;
}

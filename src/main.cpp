#include <bitset>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>

#include "Emulator.hpp"
#include "ConsoleInterface.hpp"
#include "commands/CommandsIndex.hpp"

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
    program.fill(0);
    inputFile.read(reinterpret_cast<char*>(program.data()), fileSize);

    td4::ConsoleInterface cli;
    td4::EmulatorPtr emulator{std::make_shared<td4::Emulator>(program)};
    td4::DebuggerPtr debugger{std::make_shared<td4::Debugger>(emulator)};
    bool quit{false};

    cli.RegisterCommand<td4::CommandRun>("run", debugger)
       .RegisterCommand<td4::CommandHelp>("help", debugger)
       .RegisterCommand<td4::CommandStep>("step", debugger)
       .RegisterCommand<td4::CommandQuit>("quit", debugger, &quit)
       .RegisterCommand<td4::CommandBreak>("break", debugger)
       .RegisterCommand<td4::CommandClear>("clear", debugger)
       .RegisterCommand<td4::CommandExamine>("examine", debugger);

    while (!quit) {
        std::cout << "> ";
        try {
            cli.ExecuteCommand(std::cin);
        } catch (const td4::UnknownCommand &err) {
            std::cerr << err.what() << '\n';
            std::cout << "Use \"help\" to get command reference.\n";
        }
    }
    
    return 0;
}

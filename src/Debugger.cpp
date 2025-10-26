#include "Debugger.hpp"

#include <chrono>
#include <iostream>

namespace td4 {
    Debugger::Debugger(const EmulatorPtr &emulator)
        : _emulator(emulator), _clockFrequency(0) {
        this->ClearBreaks();
    }

    void Debugger::Run(size_t steps, bool looped) {
        using namespace std::chrono;

        this->_breakpoints[0] = true;

        for (; steps > 0 || looped; --steps) {
            const Registers registers{this->_emulator->Step()};

            if (this->_breakpoints.at(registers.PC))
                break;

            steady_clock::time_point begin = steady_clock::now();
            steady_clock::time_point end = begin;
            while (this->_clockFrequency != 0.f &&
                duration_cast<milliseconds>(end - begin).count() < 1000.f / this->_clockFrequency)
                end = steady_clock::now();

            std::cout << *this->_emulator << '\n';
            std::cout << registers << std::endl;
        }
    }

    void Debugger::SetClockFrequency(float frequency) {
        this->_clockFrequency = frequency;
    }

    void Debugger::ToggleBreak(size_t instruction) {
        this->_breakpoints[instruction] = !this->_breakpoints[instruction];
    }

    void Debugger::ClearBreaks(void) {
        std::fill(this->_breakpoints.begin(), this->_breakpoints.end(), false);
    }

    EmulatorPtr Debugger::Emulator(void) {
        return this->_emulator;
    }
}
#pragma once
#include "Emulator.hpp"
#include <vector>

namespace td4 {
    class Debugger {
    public:
        Debugger(const EmulatorPtr& emulator);

        void Run(size_t steps = 0, bool looped = true);

        void SetClockFrequency(float frequency);

        void ToggleBreak(size_t instruction);

        void ClearBreaks(void);

        EmulatorPtr Emulator(void);

    private:
        std::array<bool, PROGRAM_MAX_LEN> _breakpoints;
        EmulatorPtr _emulator;
        float _clockFrequency;
    };

    using DebuggerPtr = std::shared_ptr<Debugger>;
}

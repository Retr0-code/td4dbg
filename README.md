# Debugger for TD4

A realistic debugger/emulator for TD4. It recreates all steps used in real TD4:
- Input selection;
- Addition;
- Loading;

You should use [td4c](https://github.com/Retr0-code/td4c) compiler, because the emulator uses a binary file as an imput.

## Building and extension

To build **CMake** with **Ninja** is required.
Preset `debug` compiles the debugger with no optimizations and include debug symbols.
Preset `release` enables compiler optimizations.

Commands can be added with `td4::ConsoleInterface::Register()`
method inside of `src/main.cpp` main function

# Debugger for TD4

A realistic debugger/emulator for TD4. It recreates all steps used in real TD4:
- Input selection;
- Addition;
- Loading;

You should use [td4c](https://github.com/Retr0-code/td4c) compiler, because the emulator uses a binary file as an imput.

## Console

Commands are implemented in _"game like"_ console form where all commands are variables.

### Basic commands

| Command | Values | Description |
|---------|--------|-------------|
| clock | float32 | Sets frequency of the emulator |
| step | int32 | Amount of instructions to execute |
| run | bool | Flag to continue execution |
| quit | bool | Flag to exit emulator |

## Building and extension

To build **CMake** with **Ninja** is required.
Preset `debug` compiles the debugger with no optimizations and include debug symbols.
Preset `release` enables compiler optimizations.

Commands can be added with `td4::ConsoleInterface::Register()`
method inside of `src/main.cpp` main function

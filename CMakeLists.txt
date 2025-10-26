cmake_minimum_required(VERSION 3.17.0)
project(td4dbg VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    src/Emulator.cpp
    src/Debugger.cpp
    src/Disassembler.cpp
    src/ConsoleInterface.cpp
)

add_subdirectory(src/commands)

add_library(${PROJECT_NAME}-emulator STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME}-emulator PUBLIC include)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-emulator)
target_link_libraries(${PROJECT_NAME} debugger_commands)

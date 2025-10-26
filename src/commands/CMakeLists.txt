cmake_minimum_required(VERSION 3.17.0)
project(debugger_commands)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    CommandRun.cpp
    CommandHelp.cpp
    CommandStep.cpp
    CommandQuit.cpp
    CommandBreak.cpp
    CommandClear.cpp
    CommandExamine.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${td4dbg_SOURCE_DIR}/include/commands)

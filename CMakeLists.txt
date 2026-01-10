cmake_minimum_required(VERSION 3.14)
project(RegexEngine)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.12.0
)

FetchContent_MakeAvailable(Catch2)

set(CMAKE_CXX_STANDARD 20)

add_library(RegexEngineLib
        Tokenizer/Tokenize.cpp
        Tokenizer/Tokenize.h
        AST/AST.cpp
        AST/AST.h
        NFA/NFA.cpp
        NFA/NFA.h
        AST/Constants.h
        AST/Parser.cpp
        AST/Parser.h
        NFA/State.cpp
        NFA/State.h
        NFA/NFABuilder.cpp
        NFA/Constants.h
        NFA/NFABuilder.h)

# Main application executable
add_executable(RegexEngine main.cpp)
target_link_libraries(RegexEngine PRIVATE RegexEngineLib)

# Test executable
add_executable(RegexEngineTests Tokenizer/TokenizerTests.cpp)
target_link_libraries(RegexEngineTests PRIVATE RegexEngineLib Catch2::Catch2WithMain)
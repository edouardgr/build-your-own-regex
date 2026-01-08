cmake_minimum_required(VERSION 4.1)
project(RegexEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(RegexEngine main.cpp
        Tokenizer/Tokenize.cpp
        Tokenizer/Tokenize.h
        AST/AST.cpp
        AST/AST.h
        NFA/NFA.cpp
        NFA/NFA.h)

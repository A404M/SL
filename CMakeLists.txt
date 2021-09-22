cmake_minimum_required(VERSION 3.20.2)
project(SL)

#set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
#set(CMAKE_CXX_FLAGS_DEBUG "-Ofast")
#set(CMAKE_CXX_FLAGS_RELEASE "-O4")

add_executable(SL main.cpp SL/Compiler/additional/functions.cpp SL/Compiler/additional/functions.h SL/Compiler/additional/enums.h SL/Compiler/Node/Node.cpp SL/Compiler/Node/Node.h SL/Compiler/Lexer/Lexer.cpp SL/Compiler/Lexer/Lexer.h SL/Compiler/Parser/Parser.cpp SL/Compiler/Parser/Parser.h SL/Compiler/CodeGenerator/CodeGenerator.cpp SL/Compiler/CodeGenerator/CodeGenerator.h SL/VM/Var/Var.cpp SL/VM/Var/Var.h SL/VM/Runner/Runner.cpp SL/VM/Runner/Runner.h SL/Compiler/additional/types.h)

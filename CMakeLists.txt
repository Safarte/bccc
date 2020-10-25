cmake_minimum_required(VERSION 3.17)
project(bccc)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(bccc
        src/lexer.cpp
        src/lexer.h
        src/main.cpp
        src/parser.h
        src/parser.cpp src/emitter.h src/emitter.cpp src/ast.h src/token.h src/ast.cpp src/token.cpp)

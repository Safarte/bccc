cmake_minimum_required(VERSION 3.17)
project(bccc)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(bccc
        src/classifier.h
        src/lexer.cpp
        src/lexer.h
        src/main.cpp
        src/parser.h
        src/token.h src/classifier.cpp)

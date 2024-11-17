cmake_minimum_required(VERSION 3.29)
project(CLogik C)

set(CMAKE_C_STANDARD 11)

add_executable(CLogik src/main.c
        src/lexer.c
        src/globalVariables.c
        headers/token.h
        src/token.c
        headers/lexer.h
        src/parser.c
        headers/parser.h
        src/variable.c
        headers/variable.h
        src/interpret.c
        headers/interpret.h
        src/print.c
        headers/AST.h
        src/AST.c
)

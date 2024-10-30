cmake_minimum_required(VERSION 3.29)
project(CLogik C)

set(CMAKE_C_STANDARD 11)

add_executable(CLogik src/main.c
        src/lexer.c
        headers/token.h
        src/token.c
        headers/lexer.h
        src/parser.c
        headers/parser.h
        src/variable.c
        headers/variable.h
        src/validator.c
        headers/validator.h)

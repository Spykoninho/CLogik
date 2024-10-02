cmake_minimum_required(VERSION 3.29)
project(CLogik C)

set(CMAKE_C_STANDARD 11)

add_executable(CLogik main.c
        lexer.c)

cmake_minimum_required(VERSION 3.12)
project(fifteen-puzzle C)

set(CMAKE_C_STANDARD 11)

add_executable(main Programa/src/solver/main.c)

cmake_minimum_required(VERSION 3.20)
project(Matrix C)

set(CMAKE_C_STANDARD 99)

add_executable(Matrix main.c matrix.c matrix.h)

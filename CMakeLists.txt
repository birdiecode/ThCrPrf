cmake_minimum_required(VERSION 3.27)
project(thcrprf C)

set(CMAKE_C_STANDARD 11)


add_executable(thcrprf main.c
        crypto.c
        crypto.h)

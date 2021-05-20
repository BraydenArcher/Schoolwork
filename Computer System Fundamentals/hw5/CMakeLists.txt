cmake_minimum_required(VERSION 3.17)
project(hw5)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hw5
        calc.h
        calcInteractive.c
        "../Exam 4/calcServer.c"
        calcTest.c
        "../Exam 4/csapp.c"
        csapp.h
        Makefile
        tctest.c
        tctest.h
        calc.cpp)

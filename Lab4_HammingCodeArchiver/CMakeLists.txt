cmake_minimum_required(VERSION 3.23)
project(labwork4_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(getoptions)

add_executable(labwork4
        getoptions/options.cpp
        getoptions/options.h
        lib/hamarc.cpp
        lib/hamarc.h
        main.cpp)

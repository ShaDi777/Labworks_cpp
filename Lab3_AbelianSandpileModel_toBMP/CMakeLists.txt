cmake_minimum_required(VERSION 3.23)
project(labwork_3_ShaDi777)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(getoptions)
include_directories(lib)

add_executable(labwork_3_ShaDi777
        getoptions/options.cpp
        getoptions/options.h
        lib/bmp.cpp
        lib/bmp.h
        lib/table.cpp
        lib/table.h
        main.cpp)

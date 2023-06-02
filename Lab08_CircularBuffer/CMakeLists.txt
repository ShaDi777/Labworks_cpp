cmake_minimum_required(VERSION 3.0.0)
project(labwork8 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

link_directories(lib)

add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)
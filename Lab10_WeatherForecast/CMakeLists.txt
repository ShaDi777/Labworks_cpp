cmake_minimum_required(VERSION 3.23)
project(labwork10)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)


if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()


# ==== JSON ====
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


# ==== C++ Requests ====
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)


add_executable(labwork10 main.cpp lib/TableCreation.cpp lib/structures.h lib/constants.h lib/functions.cpp)
target_link_libraries(labwork10 PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(labwork10 PRIVATE cpr::cpr)

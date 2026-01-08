# Minimum version of CMake required
cmake_minimum_required(VERSION 3.14)

# Project name and language
project(GameOfLife LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force all executables to be generated in the root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# ---------------------------
# Source files for the main library
# ---------------------------
set(SOURCES
    src/Cell.cpp
    src/Grid.cpp
    src/Game.cpp
    src/Workflow.cpp
)

# Compile sources into a library
add_library(game_lib ${SOURCES})

# Tell CMake where to find headers
target_include_directories(game_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ---------------------------
# Main executable
# ---------------------------
add_executable(GameOfLife src/main.cpp)
target_link_libraries(GameOfLife PRIVATE game_lib)
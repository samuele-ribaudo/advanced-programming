# Minimum version of CMake required
cmake_minimum_required(VERSION 3.14)

# Project name and language
project(GameOfLife LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Force all executables to be generated in the root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ---------------------------
# Library target
# ---------------------------
# Compile GameOfLife.cpp into a library
add_library(game_lib src/GameOfLife.cpp)

# Tell CMake where to find headers
target_include_directories(game_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ---------------------------
# Executable target
# ---------------------------
# Compile main.cpp into an executable, link the library
add_executable(game_cli src/main.cpp)
target_link_libraries(game_cli PRIVATE game_lib)

# ---------------------------
# Enable testing
# ---------------------------
enable_testing()


# ---------------------------
# Google Test target
# ---------------------------
# Trova la libreria statica manualmente
find_library(GTEST_LIB gtest PATHS /usr/local/lib /opt/homebrew/lib)
find_library(GTEST_MAIN_LIB gtest_main PATHS /usr/local/lib /opt/homebrew/lib)

add_executable(game_tests test/game_tests.cpp)
target_link_libraries(game_tests PRIVATE game_lib ${GTEST_LIB} ${GTEST_MAIN_LIB})
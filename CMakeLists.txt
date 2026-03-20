cmake_minimum_required(VERSION 3.20)

project(chess_board LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(chess_board)

file(GLOB_RECURSE SOURCES sources/*.cpp)
target_sources(chess_board PRIVATE ${SOURCES})

enable_testing()
add_test(NAME run_chess_board COMMAND chess_board)

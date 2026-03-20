cmake_minimum_required(VERSION 3.20)

project(chess_board LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES sources/*.cpp)

add_executable(chess_board)
target_sources(chess_board PRIVATE ${SOURCES})

add_executable(chess_board_test)
target_sources(chess_board_test PRIVATE ${SOURCES})
target_compile_definitions(chess_board_test PRIVATE MOVE_LIMIT=3)

enable_testing()

add_test(NAME run_default COMMAND chess_board_test)
add_test(NAME run_4_rooks COMMAND chess_board_test 4)
add_test(NAME run_5_rooks COMMAND chess_board_test 5)
add_test(NAME run_6_rooks COMMAND chess_board_test 6)

add_test(NAME fail_too_few COMMAND chess_board_test 3)
set_tests_properties(fail_too_few PROPERTIES WILL_FAIL TRUE)

add_test(NAME fail_too_many COMMAND chess_board_test 7)
set_tests_properties(fail_too_many PROPERTIES WILL_FAIL TRUE)

add_test(NAME fail_invalid COMMAND chess_board_test abc)
set_tests_properties(fail_invalid PROPERTIES WILL_FAIL TRUE)

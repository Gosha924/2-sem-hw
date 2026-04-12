cmake_minimum_required(VERSION 3.10)
project(csvProject C)
set(CMAKE_C_STANDART 11)

enable_testing()
add_executable(tests test.c csv.c)
add_test(NAME runTests COMMAND tests)

add_executable(exe example.c csv.c)
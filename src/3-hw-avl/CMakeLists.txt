cmake_minimum_required(VERSION 3.10)
project(airportProject C)
set(CMAKE_C_STANDART 11)

add_executable(main main.c avl.c)

enable_testing()
add_executable(tests avltest.c avl.c)
add_test(NAME runTests COMMAND tests)

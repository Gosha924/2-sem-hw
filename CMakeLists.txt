cmake_minimum_required(VERSION 3.10)
project(airportProject C)
set(CMAKE_C_STANDART 11)

add_executable(main src/3-hw-avl/main.c src/3-hw-avl/avl.c)

enable_testing()
add_executable(tests src/3-hw-avl/avltest.c src/3-hw-avl/avl.c)
add_test(NAME runTests COMMAND tests)

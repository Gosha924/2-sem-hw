cmake_minimum_required(VERSION 3.10)
project(csvProject C)
set(CMAKE_C_STANDARD 11)

enable_testing()
add_executable(tests src/1-hw-csv/test.c src/1-hw-csv/csv.c)
add_test(NAME runTests COMMAND tests)

add_executable(exe src/1-hw-csv/example.c src/1-hw-csv/csv.c)
cmake_minimum_required(VERSION 3.10)
project(goverments C)
set(CMAKE_C_STANDART 11)

add_executable(main main.c graph.c queue.c)

enable_testing()
add_test(NAME integration_tests
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runTests.sh $<TARGET_FILE:main>
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
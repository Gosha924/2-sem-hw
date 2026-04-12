cmake_minimum_required(VERSION 3.10)
project(goverments C)
set(CMAKE_C_STANTARD 11)

add_executable(main src/4_hw_goverments/main.c src/4_hw_goverments/graph.c src/4_hw_goverments/queue.c)

enable_testing()
add_test(NAME integration_tests
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/src/4_hw_goverments/runTests.sh $<TARGET_FILE:main>
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
cmake_minimum_required(VERSION 3.12)
project(interpreter CXX)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)

option(COMPILE_TESTS "Compile the tests" OFF)
if (COMPILE_TESTS)
    add_subdirectory(tests)
endif(COMPILE_TESTS)

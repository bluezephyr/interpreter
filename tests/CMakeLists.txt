find_package(PkgConfig REQUIRED)
pkg_search_module(CPPUTEST REQUIRED cpputest>=3.8)
message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")

include_directories(../src)

add_executable(interpreter_test
        main.cpp
        LexerTest.cpp)

target_link_libraries(interpreter_test
        interpreterlib
        CppUTest
        CppUTestExt
        )

# Find CppUTest installation
if (DEFINED CPPUTEST_HOME)
    # Use only the version in the CPPUTEST_HOME folder if it is defined
    message(STATUS "Searching for CppUTest in: ${CPPUTEST_HOME}")
    find_package(CppUTest 3.8 REQUIRED NO_DEFAULT_PATH CONFIG HINTS ${CPPUTEST_HOME})
else()
    message(STATUS "Searching for installed version of CppUTest")
    find_package(CppUTest 3.8 REQUIRED CONFIG)
endif()

message(STATUS "CppUTest version: ${CppUTest_VERSION}")
message(STATUS "CppUTest dir: ${CppUTest_DIR}")

include_directories(../src)

add_executable(interpreter_test
    main.cpp
    LexerTest.cpp
    ParserTest.cpp )

target_link_libraries(interpreter_test
    interpreterlib
    CppUTest
    CppUTestExt )

add_test(interpreter interpreter_test)

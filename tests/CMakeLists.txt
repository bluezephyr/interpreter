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

add_executable(lexer_test LexerTest.cpp)
target_link_libraries(lexer_test lexer CppUTest CppUTestExt)

add_executable(parser_test ParserTest.cpp)
target_link_libraries(parser_test lexer parser CppUTest CppUTestExt)

add_test(lexer lexer_test)
add_test(parser parser_test)

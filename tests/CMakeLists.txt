# Find CppUTest installation
find_library(CPPUTESTLIB NAMES CppUTest HINTS ${CPPUTEST_HOME}/cpputest_build/lib)
find_library(CPPUTESTEXTLIB NAMES CppUTestExt HINTS ${CPPUTEST_HOME}/cpputest_build/lib)
message("CppUTest lib: ${CPPUTESTLIB}")
message("CppUTestExt lib: ${CPPUTESTEXTLIB}")

add_executable(token_test TokenTest.cpp)
target_link_libraries(token_test token CppUTest CppUTestExt)

add_executable(lexer_test LexerTest.cpp)
target_link_libraries(lexer_test lexer CppUTest CppUTestExt)

add_executable(ast_test AstTest.cpp)
target_link_libraries(ast_test ast CppUTest CppUTestExt)

add_executable(parser_test ParserTest.cpp)
target_link_libraries(parser_test parser CppUTest CppUTestExt)

add_executable(object_test ObjectTest.cpp)
target_link_libraries(object_test object CppUTest CppUTestExt)

add_executable(ast_printer_test AstPrinterTest.cpp)
target_link_libraries(ast_printer_test astPrinter parser CppUTest CppUTestExt)

add_executable(eval_test EvalTest.cpp)
target_link_libraries(eval_test evaluator parser CppUTest CppUTestExt)

add_test(ast ast_test)
add_test(token token_test)
add_test(lexer lexer_test)
add_test(parser parser_test)
add_test(object object_test)
add_test(printer ast_printer_test)
add_test(eval eval_test)

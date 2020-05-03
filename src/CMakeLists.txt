add_library(token
    Token.h
    Token.cpp)
target_include_directories(token PUBLIC ../src)

add_library(lexer
    Lexer.h
    Lexer.cpp)
target_include_directories(lexer PUBLIC ../src)
target_link_libraries(lexer token)

add_library(object
    Object.h
    Object.cpp)
target_include_directories(object PUBLIC ../src)

add_library(ast
    Ast.h
    Ast.cpp)
target_include_directories(ast PUBLIC ../src)
target_link_libraries(ast token object)

add_library(parser
    Exceptions.h
    Parser.h
    Parser.cpp)
target_include_directories(parser PUBLIC ../src)
target_link_libraries(parser ast lexer)

# The interpreter
add_executable(interpreter main.cpp)
target_link_libraries(interpreter token lexer parser)

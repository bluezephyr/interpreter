add_library(token
    Token.h
    Token.cpp)

add_library(lexer
    Lexer.h
    Lexer.cpp)

add_library(parser
    ast.h
    ast.cpp
    Exceptions.h
    Parser.h
    Parser.cpp)

# The interpreter
add_executable(interpreter main.cpp)
target_link_libraries(interpreter token lexer parser)

add_library(lexer
    Lexer.h
    Lexer.cpp
    Token.h
    Token.cpp
    ast.h
    ast.cpp)

add_library(parser
    Parser.h
    Parser.cpp )

# The interpreter
add_executable(interpreter main.cpp)
target_link_libraries(interpreter lexer parser)

add_library(token
    Token.h
    Token.cpp)

add_library(lexer
    Lexer.h
    Lexer.cpp)

add_library(parser
    Parser.h
    Parser.cpp
    ast.h
    ast.cpp)

# The interpreter
add_executable(interpreter main.cpp)
target_link_libraries(interpreter token lexer parser)

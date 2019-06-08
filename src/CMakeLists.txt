add_library(interpreterlib
        Lexer.h
        Lexer.cpp
        Token.h
        Token.cpp
        )

add_executable(interpreter
        main.cpp
        )

target_link_libraries(interpreter interpreterlib)
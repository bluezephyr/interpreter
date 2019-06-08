/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <string>

class Token
{
public:
    enum TokenType {
        ILLEGAL,
        ENDOFFILE,
        IDENTIFIER,
        INT,
        ASSIGN,
        PLUS,
        COMMA,
        SEMICOLON,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        FUNCTION,
        LET
    };

    Token(TokenType type, std::string *literal);
    ~Token();

    enum TokenType type;
    std::string *literal;
};



#endif //INTERPRETER_TOKEN_H

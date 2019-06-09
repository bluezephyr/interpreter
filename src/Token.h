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
        MINUS,
        BANG,
        ASTERISK,
        SLASH,
        LT,
        GT,
        EQ,
        NEQ,
        COMMA,
        SEMICOLON,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        FUNCTION,
        LET
    };

    explicit Token(std::string *literal);
    Token(TokenType type, std::string *literal);
    ~Token();

    enum TokenType type;
    std::string *literal;

private:
    static TokenType lookUpType(std::string *string);
};



#endif //INTERPRETER_TOKEN_H

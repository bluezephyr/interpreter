/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef INTERPRETER_LEXER_H
#define INTERPRETER_LEXER_H

#include "Token.h"

class Lexer
{
public:
    explicit Lexer(const char*);
    virtual ~Lexer();

    Token* nextToken();

private:
    const char *input;
    int curPos;
    int readPos;
    char currentChar;

    void readChar();
    char peekChar();
    static bool isLetter(char l);
    std::string *createString(int start, int end);
    std::string *readIdentifier();

    Token *readSingleCharToken(Token::TokenType type);
};

#endif //INTERPRETER_LEXER_H

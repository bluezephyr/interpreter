/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Lexer.h"

Lexer::Lexer(const char *input)
{
    this->input = input;
    curPos = 0;
    readPos = 0;
    c = 0;
    readChar();
}

Token* Lexer::nextToken()
{
    Token *token;

    switch (c)
    {
        case 0:
           token = new Token(Token::ENDOFFILE, new std::string());
           break;

        case '=':
            token = new Token(Token::ASSIGN, createString(curPos));
            break;

        case '+':
            token = new Token(Token::PLUS, createString(curPos));
            break;

        case '(':
            token = new Token(Token::LPAREN, createString(curPos));
            break;

        case ')':
            token = new Token(Token::RPAREN, createString(curPos));
            break;

        case '[':
            token = new Token(Token::LBRACE, createString(curPos));
            break;

        case ']':
            token = new Token(Token::RBRACE, createString(curPos));
            break;

        case ',':
            token = new Token(Token::COMMA, createString(curPos));
            break;

        case ';':
            token = new Token(Token::SEMICOLON, createString(curPos));
            break;

        default:
            token = new Token(Token::ILLEGAL, createString(curPos));
            break;
    }

    return token;
}

void Lexer::readChar()
{
    c = input[readPos];
    curPos = readPos++;
}

std::string *Lexer::createString(int pos)
{
    auto s = new std::string(1, input[pos]);
    return s;
}

Lexer::~Lexer()
= default;


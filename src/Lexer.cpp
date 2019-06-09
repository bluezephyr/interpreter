/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Lexer.h"

Lexer::Lexer(const char *input) : input {input}
{
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

        case '-':
            token = new Token(Token::MINUS, createString(curPos));
            break;

        case '!':
            token = new Token(Token::BANG, createString(curPos));
            break;

        case '*':
            token = new Token(Token::ASTERISK, createString(curPos));
            break;

        case '/':
            token = new Token(Token::SLASH, createString(curPos));
            break;

        case '<':
            token = new Token(Token::LT, createString(curPos));
            break;

        case '>':
            token = new Token(Token::GT, createString(curPos));
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
            if (isLetter(c))
            {
                token = new Token(readIdentifier());
            }
            else
            {
                token = new Token(Token::ILLEGAL, createString(curPos));
            }
            break;
    }

    readChar();
    return token;
}

void Lexer::readChar()
{
    c = input[readPos];
    curPos = readPos++;
}

bool Lexer::isLetter(char l)
{
    return ('a' <= l && l <= 'z') || ('A' <= l && l <= 'Z') || (l == '_');
}

std::string *Lexer::createString(int pos)
{
    auto s = new std::string(1, input[pos]);
    return s;
}

std::string *Lexer::readIdentifier()
{
    int start = curPos;
    while (isLetter(c))
    {
        readChar();
    }

    return new std::string(input, curPos-start);
}

Lexer::~Lexer()
= default;


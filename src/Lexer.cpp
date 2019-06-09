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
    currentChar = 0;
    readChar();
}

Token* Lexer::nextToken()
{
    Token *token;

    switch (currentChar)
    {
        case 0:
            token = new Token(Token::ENDOFFILE, new std::string());
            readChar();
            break;

        case '=':
            if (peekChar() == '=')
            {
                auto start = curPos;
                readChar();
                token = new Token(Token::EQ, createString(start, curPos));
            }
            else
            {
                token = readSingleCharToken(Token::ASSIGN);
            }
            break;

        case '+':
            token = readSingleCharToken(Token::PLUS);
            break;

        case '-':
            token = readSingleCharToken(Token::MINUS);
            break;

        case '!':
            if (peekChar() == '=')
            {
                auto start = curPos;
                readChar();
                token = new Token(Token::NEQ, createString(start, curPos));
            }
            else
            {
                token = readSingleCharToken(Token::BANG);
            }
            break;

        case '*':
            token = readSingleCharToken(Token::ASTERISK);
            break;

        case '/':
            token = readSingleCharToken(Token::SLASH);
            break;

        case '<':
            token = readSingleCharToken(Token::LT);
            break;

        case '>':
            token = readSingleCharToken(Token::GT);
            break;

        case '(':
            token = readSingleCharToken(Token::LPAREN);
            break;

        case ')':
            token = readSingleCharToken(Token::RPAREN);
            break;

        case '[':
            token = readSingleCharToken(Token::LBRACE);
            break;

        case ']':
            token = readSingleCharToken(Token::RBRACE);
            break;

        case ',':
            token = readSingleCharToken(Token::COMMA);
            break;

        case ';':
            token = readSingleCharToken(Token::SEMICOLON);
            break;

        default:
            if (isLetter(currentChar))
            {
                token = new Token(readIdentifier());
            }
            else
            {
                token = readSingleCharToken(Token::ILLEGAL);
            }
            break;
    }

    return token;
}

Token *Lexer::readSingleCharToken(Token::TokenType type)
{
    auto *token = new Token(type, createString(curPos, curPos));
    readChar();
    return token;
}

void Lexer::readChar()
{
    currentChar = input[readPos];
    curPos = readPos++;
}

char Lexer::peekChar()
{
    return input[readPos];
}

bool Lexer::isLetter(char l)
{
    return ('a' <= l && l <= 'z') || ('A' <= l && l <= 'Z') || (l == '_');
}

std::string *Lexer::createString(int start, int end)
{
    auto s = new std::string(input+start, end-start+1);
    return s;
}

std::string *Lexer::readIdentifier()
{
    int start = curPos;
    while (isLetter(currentChar))
    {
        readChar();
    }
    return new std::string(input+start, curPos-start);
}

Lexer::~Lexer()
= default;


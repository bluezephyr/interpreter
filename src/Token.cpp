/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Token.h"

Token::Token(const std::string& literal)
{
    type = lookUpType(literal);
    this->literal = std::make_shared<std::string>(literal);
}

Token::Token(Token::TokenType type, const std::string& literal) : type {type}
{
    this->literal = std::make_shared<std::string>(literal);
}

Token::~Token() = default;

Token::TokenType Token::lookUpType(const std::string& tokenString)
{
    if (tokenString == std::string("let"))
    {
        return LET;
    }

    if (tokenString == std::string("fn"))
    {
        return FUNCTION;
    }

    if (tokenString == std::string("true"))
    {
        return TRUE;
    }

    if (tokenString == std::string("false"))
    {
        return FALSE;
    }

    if (tokenString == std::string("if"))
    {
        return IF;
    }

    if (tokenString == std::string("else"))
    {
        return ELSE;
    }

    if (tokenString == std::string("return"))
    {
        return RETURN;
    }

    // Not a keyword
    return IDENTIFIER;
}

std::string Token::getTypeString()
{
    switch (type)
    {
        case ILLEGAL:
            return std::string("ILLEGAL");
        case ENDOFFILE:
            return std::string("EOF");
        case IDENTIFIER:
            return std::string("IDENTIFIER");
        case INT:
            return std::string("INT");
        case ASSIGN:
            return std::string("ASSIGN");
        case PLUS:
            return std::string("PLUS");
        case MINUS:
            return std::string("MINUS");
        case BANG:
            return std::string("BANG");
        case ASTERISK:
            return std::string("ASTERISK");
        case SLASH:
            return std::string("SLASH");
        case LT:
            return std::string("LT");
        case GT:
            return std::string("GT");
        case EQ:
            return std::string("EQ");
        case NEQ:
            return std::string("NEQ");
        case COMMA:
            return std::string("COMMA");
        case SEMICOLON:
            return std::string("SEMICOLON");
        case LPAREN:
            return std::string("LPAREN");
        case RPAREN:
            return std::string("RPAREN");
        case LBRACE:
            return std::string("LBRACE");
        case RBRACE:
            return std::string("RBRACE");
        case FUNCTION:
            return std::string("FN");
        case LET:
            return std::string("LET");
        case TRUE:
            return std::string("TRUE");
        case FALSE:
            return std::string("FALSE");
        case IF:
            return std::string("IF");
        case ELSE:
            return std::string("ELSE");
        case RETURN:
            return std::string("RETURN");
    }
    return std::string();
}


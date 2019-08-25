/*
 * Copyright (c) 2019 Blue Zephyr
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


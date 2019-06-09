/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Token.h"

Token::Token(std::string *literal) : literal {literal}
{
    type = lookUpType(literal);
}

Token::Token(Token::TokenType type, std::string *literal) : type {type}, literal {literal}
{}

Token::~Token()
{
    delete(literal);
}

Token::TokenType Token::lookUpType(std::string *string)
{
    if (*string == std::string("let"))
    {
        return LET;
    }
    if (*string == std::string("fn"))
    {
        return FUNCTION;
    }

    // Not a keyword
    return IDENTIFIER;
}



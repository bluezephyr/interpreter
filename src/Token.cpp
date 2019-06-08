/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Token.h"

Token::Token(Token::TokenType type, std::string *literal) : type {type}, literal {literal}
{}

Token::~Token()
{
    delete(literal);
}


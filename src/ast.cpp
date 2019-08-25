/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "ast.h"

// Identifier
Identifier::Identifier(std::unique_ptr<Token> token) : token(std::move(token))
{
    value = this->token->literal;
}

// Statements
LetStatement::LetStatement() : token(nullptr), identifier(nullptr), expression(nullptr) {}

LetStatement::LetStatement(std::unique_ptr<Token> token) :
        token(std::move(token)),
        identifier(nullptr),
        expression(nullptr) {}

LetStatement::~LetStatement() = default;

// Expressions
Expression::~Expression() = default;


// Program
Program::~Program() = default;

void Program::addStatement(std::unique_ptr<Statement> statement)
{
    statements.push_back(std::move(statement));
}


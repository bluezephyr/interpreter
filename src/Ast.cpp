/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Ast.h"

// Identifier
Identifier::Identifier(std::unique_ptr<Token> token) : token(std::move(token))
{
    value = this->token->literal;
}

std::string Identifier::string()
{
    return std::string(*value);
}

// Statements
LetStatement::LetStatement() : token(nullptr), identifier(nullptr), expression(nullptr) {}
LetStatement::LetStatement(std::unique_ptr<Token> token) :
        token(std::move(token)),
        identifier(nullptr),
        expression(nullptr) {}

std::string LetStatement::string()
{
    std::string statement = *(token->literal) + " ";
    statement += identifier->string();
    statement += " = ";
    if (expression != nullptr)
    {
        statement += expression->string();
    }
    statement += ";";
    return statement;
}

ReturnStatement::ReturnStatement() : token(nullptr), expression(nullptr) {}
ReturnStatement::ReturnStatement(std::unique_ptr<Token> token) :
        token(std::move(token)),
        expression(nullptr) {}

std::string ReturnStatement::string()
{
    std::string statement = *(token->literal) + " ";
    if (expression != nullptr)
    {
        statement += expression->string();
    }
    statement += ";";
    return statement;
}

// Expressions
ExpressionStatement::ExpressionStatement() : token(nullptr), expression(nullptr) {}
ExpressionStatement::ExpressionStatement(std::unique_ptr<Token> token) :
        token(std::move(token)),
        expression(nullptr) {}
std::string ExpressionStatement::string()
{
    std::string statement;
    if (expression != nullptr)
    {
        statement += expression->string();
    }
    return statement;
}

// Program
void Program::addStatement(std::unique_ptr<Statement> statement)
{
    statements.push_back(std::move(statement));
}

std::string Program::string()
{
    std::string programString;
    for (auto const& statement : statements)
    {
        programString += statement->string() + "\n";
    }
    return programString;
}


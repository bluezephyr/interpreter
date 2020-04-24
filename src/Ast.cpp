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

// Integer
Integer::Integer(std::unique_ptr<Token> token) : token(std::move(token))
{
    value = 0;
}

std::string Integer::string()
{
    return std::to_string(value);
}

// Boolean
Boolean::Boolean(std::unique_ptr<Token> token, bool value) :
        token(std::move(token)),
        value{value} {}

std::string Boolean::string()
{
    if (value)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

// PrefixExpression
PrefixExpression::PrefixExpression(std::unique_ptr<Token> token) :
        token(std::move(token)),
        right{nullptr} {}

std::string PrefixExpression::string()
{
    return std::string("(" + op + right->string() + ")");
}

// InfixExpression
InfixExpression::InfixExpression(std::unique_ptr<Token> token) :
        left{nullptr},
        token(std::move(token)),
        right{nullptr} {}

std::string InfixExpression::string()
{
    return std::string("(" + left->string() + " " + op + " " + right->string() + ")");
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
ExpressionStatement::ExpressionStatement() : expression(nullptr) {}
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
void Program::addStatement(std::shared_ptr<Statement> statement)
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


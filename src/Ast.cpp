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

std::shared_ptr<Object> Identifier::eval()
{
   return nullptr;
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

std::shared_ptr<Object> Integer::eval()
{
    return std::make_shared<IntegerObject>(value);
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

std::shared_ptr<Object> Boolean::eval()
{
    return std::make_shared<BooleanObject>(value);
}

// Function
Function::Function(std::unique_ptr<Token> token) :
        token(std::move(token)) {}

std::string Function::string()
{
    auto expression = std::string(*token->literal + "(");
    for (const auto& parameter: parameters)
    {
        expression += parameter->string();
        if(parameter != parameters.back())
        {
            expression += ", ";
        }
    }
    expression += ") { ";
    expression += body->string();
    expression += " }";

    return expression;
}

std::shared_ptr<Object> Function::eval()
{
    return nullptr;
}

// CallExpression
CallExpression::CallExpression(std::unique_ptr<Token> token) :
        token(std::move(token)),
        function(nullptr) {}

std::string CallExpression::string()
{
    auto expression = std::string(function->string() + "(");
    for (const auto& argument: arguments)
    {
        expression += argument->string();
        if(argument != arguments.back())
        {
            expression += ", ";
        }
    }
    expression += ")";
    return expression;
}

std::shared_ptr<Object> CallExpression::eval()
{
    return nullptr;
}

// PrefixExpression
PrefixExpression::PrefixExpression(std::unique_ptr<Token> token) :
        token(std::move(token)),
        right{nullptr} {}

std::string PrefixExpression::string()
{
    return std::string("(" + op + right->string() + ")");
}

std::shared_ptr<Object> PrefixExpression::eval()
{
    auto rightEvaluated = right->eval();
    if(token->type == Token::BANG)
    {
        return rightEvaluated->evalBangPrefixExpression();
    }
    else if (token->type == Token::MINUS)
    {
        return rightEvaluated->evalMinusPrefixExpression();
    }
    return nullptr;
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

std::shared_ptr<Object> InfixExpression::eval()
{
    return nullptr;
}

// IfExpression
IfExpression::IfExpression(std::unique_ptr<Token> token) :
        token(std::move(token)),
        condition(nullptr),
        consequence(nullptr),
        alternative(nullptr) {}

std::string IfExpression::string()
{
    auto expression = std::string(*token->literal + " " + condition->string() + " { " +
                                  consequence->string() + " }");
    if(alternative != nullptr)
    {
        expression += " else { " + alternative->string() + " }";
    }

    expression += "\n";
    return expression;
}

std::shared_ptr<Object> IfExpression::eval()
{
    return nullptr;
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

std::shared_ptr<Object> LetStatement::eval()
{
    return nullptr;
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

std::shared_ptr<Object> ReturnStatement::eval()
{
    return nullptr;
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

std::shared_ptr<Object> ExpressionStatement::eval()
{
    return expression->eval();
}

// BlockStatement
std::string BlockStatement::string()
{
    std::string block;
    for (auto const& statement : statements)
    {
        block += statement->string() + "\n";
    }
    return block;
}

std::shared_ptr<Object> BlockStatement::eval()
{
    return nullptr;
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

std::shared_ptr<Object> Program::eval()
{
    std::shared_ptr<Object> result;
    for(const auto& statement: statements)
    {
        result = statement->eval();
    }
    return result;
}


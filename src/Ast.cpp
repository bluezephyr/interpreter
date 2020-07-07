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

void Identifier::accept(AstVisitor &visitor)
{
    visitor.visitIdentifier(*this);
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

void Integer::accept(AstVisitor &visitor)
{
    visitor.visitInteger(*this);
}

// Boolean
Boolean::Boolean(std::unique_ptr<Token> token, bool value) :
        token(std::move(token)),
        value{value} {}

std::string Boolean::string()
{
    return value ? "true" : "false";
}

void Boolean::accept(AstVisitor &visitor)
{
    visitor.visitBoolean(*this);
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

void Function::accept(AstVisitor &visitor)
{
    visitor.visitFunction(*this);
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

void CallExpression::accept(AstVisitor &visitor)
{
    visitor.visitCallExpression(*this);
}

// PrefixExpression
PrefixExpression::PrefixExpression(std::unique_ptr<Token> token) :
        token(std::move(token)),
        right{nullptr} {}

std::string PrefixExpression::string()
{
    return std::string("(" + op + right->string() + ")");
}

void PrefixExpression::accept(AstVisitor &visitor)
{
    visitor.visitPrefixExpression(*this);
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

void InfixExpression::accept(AstVisitor &visitor)
{
    visitor.visitInfixExpression(*this);
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

void IfExpression::accept(AstVisitor &visitor)
{
    visitor.visitIfExpression(*this);
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

void LetStatement::accept(AstVisitor &visitor)
{
    visitor.visitLetStatement(*this);
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

void ReturnStatement::accept(AstVisitor &visitor)
{
    visitor.visitReturnStatement(*this);
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

void ExpressionStatement::accept(AstVisitor &visitor)
{
    visitor.visitExpressionStatement(*this);
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

void BlockStatement::accept(AstVisitor &visitor)
{
    visitor.visitBlockStatement(*this);
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

void Program::accept(AstVisitor &visitor)
{
    visitor.visitProgram(*this);
}


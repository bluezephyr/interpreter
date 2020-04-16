/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef INTERPRETER_AST_H
#define INTERPRETER_AST_H

#include <memory>
#include <string>
#include <vector>
#include "Token.h"

class Node
{
public:
    virtual ~Node() = default;
    virtual std::string string() = 0;
};

class Expression : public Node
{
public:
    ~Expression() override = default;
    std::string string() override = 0;
};

class Identifier : public Expression
{
public:
    explicit Identifier(std::unique_ptr<Token> token);
    ~Identifier() override = default;
    std::string string() override;

    std::shared_ptr<Token> token;
    std::shared_ptr<std::string> value;
};

class Integer : public Expression
{
public:
    explicit Integer(std::unique_ptr<Token> token);
    ~Integer() override = default;
    std::string string() override;

    std::shared_ptr<Token> token;
    int64_t value;
};

class PrefixExpression : public Expression
{
public:
    explicit PrefixExpression(std::unique_ptr<Token> token);
    ~PrefixExpression() override = default;
    std::string string() override;

    std::shared_ptr<Token> token;
    std::string op;
    std::shared_ptr<Expression> right;

};

class InfixExpression : public Expression
{
public:
    explicit InfixExpression(std::unique_ptr<Token> token);
    ~InfixExpression() override = default;
    std::string string() override;

    std::shared_ptr<Token> token;
    std::shared_ptr<Expression> left;
    std::string op;
    std::shared_ptr<Expression> right;

};

// Statements
class Statement : public Node
{
public:
    ~Statement() override = default;
    std::string string() override = 0;
};

class LetStatement : public Statement
{
public:
    LetStatement();
    explicit LetStatement(std::unique_ptr<Token> token);
    ~LetStatement() override = default;
    std::string string() override;
    std::shared_ptr<Token> token;
    std::shared_ptr<Identifier> identifier;
    std::shared_ptr<Expression> expression;
};

class ReturnStatement : public Statement
{
public:
    ReturnStatement();
    explicit ReturnStatement(std::unique_ptr<Token> token);
    ~ReturnStatement() override = default;
    std::string string() override;
    std::shared_ptr<Token> token;
    std::shared_ptr<Expression> expression;
};

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement();
    ~ExpressionStatement() override = default;
    std::string string() override;
    std::shared_ptr<Expression> expression;
};

// Program
class Program : public Node
{
public:
    Program() = default;
    ~Program() override = default;
    void addStatement(std::shared_ptr<Statement> statement);
    std::string string() override;
    std::vector<std::shared_ptr<Statement>> statements;
};

#endif //INTERPRETER_AST_H

/*
 * Copyright (c) 2019 Blue Zephyr
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
};

class Identifier
{
public:
    explicit Identifier(std::unique_ptr<Token> token);
    std::shared_ptr<Token> token;
    std::shared_ptr<std::string> value;
};

class Expression : public Node
{
public:
    ~Expression() override;
};

// Statements
class Statement : public Node
{
public:
    ~Statement() override = default;
};

class LetStatement : public Statement
{
public:
    LetStatement();
    explicit LetStatement(std::unique_ptr<Token> token);
    ~LetStatement() override;
    std::shared_ptr<Token> token;
    std::shared_ptr<Identifier> identifier;
private:
    std::unique_ptr<Expression> expression;
};

class ReturnStatement : public Statement
{
public:
    ReturnStatement();
    explicit ReturnStatement(std::unique_ptr<Token> token);
    ~ReturnStatement() override;
    std::shared_ptr<Token> token;
private:
    std::unique_ptr<Expression> expression;
};

// Program
class Program
{
public:
    Program() = default;
    ~Program();
    void addStatement(std::unique_ptr<Statement> statement);
    std::vector<std::shared_ptr<Statement>> statements;
};

#endif //INTERPRETER_AST_H

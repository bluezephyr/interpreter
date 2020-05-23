/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#ifndef INTERPRETER_ASTPRINTER_H
#define INTERPRETER_ASTPRINTER_H

#include <string>
#include <memory>
#include <stack>
#include "Ast.h"

class ControlToken : public Node
{
public:
    explicit ControlToken(std::string);
    std::string string() override;
    std::shared_ptr<Object> eval() override;
    void accept(AstVisitor &visitor) override;

private:
    std::string _string;
};

class AstPrinter : public AstVisitor
{
public:
    AstPrinter();
    std::string printCode(const std::shared_ptr<Node>&);

    void visitIdentifier(Identifier &identifier) override;
    void visitInteger(Integer &integer) override;
    void visitBoolean(Boolean &boolean) override;
    void visitFunction(Function &function) override;
    void visitCallExpression(CallExpression &expression) override;
    void visitPrefixExpression(PrefixExpression &expression) override;
    void visitInfixExpression(InfixExpression &expression) override;
    void visitIfExpression(IfExpression &expression) override;
    void visitLetStatement(LetStatement &statement) override;
    void visitReturnStatement(ReturnStatement &statement) override;
    void visitExpressionStatement(ExpressionStatement &statement) override;
    void visitBlockStatement(BlockStatement &statement) override;
    void visitProgram(Program &program) override;
    void visitControlToken(ControlToken &controlToken) override;

private:
    std::string output;
    std::stack<std::shared_ptr<Node>> visitStack;
    void addStatements(std::vector<std::shared_ptr<Statement>> statements);
};


#endif //INTERPRETER_ASTPRINTER_H

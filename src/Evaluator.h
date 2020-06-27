/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#ifndef INTERPRETER_EVALUATOR_H
#define INTERPRETER_EVALUATOR_H

#include <stack>
#include "AstVisitor.h"
#include "Object.h"
#include "Ast.h"

class Evaluator : public AstVisitor
{
public:
    Evaluator();
    void visitIdentifier(Identifier&) override;
    void visitInteger(Integer&) override;
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
    std::shared_ptr<Object> eval(const std::shared_ptr<Node>& startNode);

private:
    bool goingUp;
    bool breakBlock;
    std::stack<std::shared_ptr<Node>> visitStack;
    std::stack<std::shared_ptr<Object>> evalStack;
    void addStatements(std::vector<std::shared_ptr<Statement>> statements);
    static std::shared_ptr<Object> evalIntegerInfixExpression(Token::TokenType op, IntegerObject *left,
                                                              IntegerObject *right);
    static std::shared_ptr<Object> evalBooleanInfixExpression(Token::TokenType op, BooleanObject *left,
                                                              BooleanObject *right);
    static bool isTruthy(const std::shared_ptr<Object>& object);

};


#endif //INTERPRETER_EVALUATOR_H

/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#ifndef INTERPRETER_EVALUATOR_H
#define INTERPRETER_EVALUATOR_H

#include "AstVisitor.h"
#include "Object.h"

class Evaluator : public AstVisitor
{
public:
    Evaluator() = default;
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
    std::shared_ptr<Object> eval(Program &program);
};


#endif //INTERPRETER_EVALUATOR_H

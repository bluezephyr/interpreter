/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#ifndef INTERPRETER_ASTVISITOR_H
#define INTERPRETER_ASTVISITOR_H

#include <memory>

class Identifier;
class Integer;
class Boolean;
class Function;
class CallExpression;
class PrefixExpression;
class InfixExpression;
class IfExpression;
class LetStatement;
class ReturnStatement;
class ExpressionStatement;
class BlockStatement;
class Program;

// The ControlToken is not part of the actual AST. It is used by the visitor
// to control the path when the tree is traversed. The use of the ControlToken
// is optional and each visitor has make its own implementation.
class ControlToken;

class AstVisitor
{
public:
    virtual void visitIdentifier(Identifier&) = 0;
    virtual void visitInteger(Integer&) = 0;
    virtual void visitBoolean(Boolean&) = 0;
    virtual void visitFunction(Function&) = 0;
    virtual void visitCallExpression(CallExpression&) = 0;
    virtual void visitPrefixExpression(PrefixExpression&) = 0;
    virtual void visitInfixExpression(InfixExpression&) = 0;
    virtual void visitIfExpression(IfExpression&) = 0;
    virtual void visitLetStatement(LetStatement&) = 0;
    virtual void visitReturnStatement(ReturnStatement&) = 0;
    virtual void visitExpressionStatement(ExpressionStatement&) = 0;
    virtual void visitBlockStatement(BlockStatement&) = 0;
    virtual void visitProgram(Program&) = 0;
    virtual void visitControlToken(ControlToken&) = 0;
};

#endif //INTERPRETER_ASTVISITOR_H

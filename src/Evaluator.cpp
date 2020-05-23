/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include "Evaluator.h"
#include "Ast.h"

std::shared_ptr<Object> Evaluator::eval(Program &program)
{
    std::shared_ptr<Object> result;
    for (const auto& statement : program.statements)
    {
        statement->accept(*this);
    }
    return result;
}

void Evaluator::visitIdentifier(Identifier &identifier)
{
}

void Evaluator::visitInteger(Integer &integer)
{
    //return std::make_shared<IntegerObject>(integer.value);
}

void Evaluator::visitBoolean(Boolean &boolean)
{

}

void Evaluator::visitFunction(Function &function)
{

}

void Evaluator::visitCallExpression(CallExpression &expression)
{

}

void Evaluator::visitPrefixExpression(PrefixExpression &expression)
{

}

void Evaluator::visitInfixExpression(InfixExpression &expression)
{

}

void Evaluator::visitIfExpression(IfExpression &expression)
{

}

void Evaluator::visitLetStatement(LetStatement &statement)
{

}

void Evaluator::visitReturnStatement(ReturnStatement &statement)
{

}

void Evaluator::visitExpressionStatement(ExpressionStatement &statement)
{
    //return statement.expression->accept(*this);
}

void Evaluator::visitBlockStatement(BlockStatement &statement)
{

}

void Evaluator::visitProgram(Program &program)
{

}

void Evaluator::visitControlToken(ControlToken &controlToken)
{

}


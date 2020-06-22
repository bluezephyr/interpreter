/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include "Ast.h"
#include "ControlToken.h"
#include "Evaluator.h"

Evaluator::Evaluator() : result(nullptr), goingUp(false) {}

std::shared_ptr<Object> Evaluator::eval(const std::shared_ptr<Node>& startNode)
{
    visitStack.push(startNode);

    // Visit all nodes in the visitList - nodes are added and removed dynamically
    while (!visitStack.empty())
    {
        std::shared_ptr<Node> node = visitStack.top();
        visitStack.pop();
        node->accept(*this);
    }

    return result;
}

void Evaluator::visitIdentifier(Identifier &identifier)
{
}

void Evaluator::visitInteger(Integer &integer)
{
    this->result = std::make_shared<IntegerObject>(integer.value);
}

void Evaluator::visitBoolean(Boolean &boolean)
{
    this->result = std::make_shared<BooleanObject>(boolean.value);
}

void Evaluator::visitFunction(Function &function)
{

}

void Evaluator::visitCallExpression(CallExpression &expression)
{

}

void Evaluator::visitPrefixExpression(PrefixExpression &expression)
{
    if(goingUp)
    {
        goingUp = false;
        if(expression.token->type == Token::BANG)
        {
            result = result->evalBangPrefixExpression();
        }
        else if (expression.token->type == Token::MINUS)
        {
            result = result->evalMinusPrefixExpression();
        }
    }
    else
    {
        visitStack.push(std::make_shared<PrefixExpression>(expression));
        visitStack.push(std::make_shared<ControlToken>(""));
        visitStack.push(expression.right);
    }
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
    visitStack.push(statement.expression);
}

void Evaluator::visitBlockStatement(BlockStatement &statement)
{

}

void Evaluator::visitProgram(Program &program)
{
    addStatements(program.statements);
}

void Evaluator::visitControlToken(ControlToken &controlToken)
{
    goingUp = true;
}

void Evaluator::addStatements(std::vector<std::shared_ptr<Statement>> statements)
{
    // Push all statements to be visited in the reverse order
    for(auto statement = statements.rbegin(); statement != statements.rend(); statement++)
    {
        visitStack.push(*statement);
    }
}



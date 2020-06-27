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

Evaluator::Evaluator() : goingUp(false) {}

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

    auto result = evalStack.top();
    evalStack.pop();

    return result;
}

void Evaluator::visitIdentifier(Identifier &identifier)
{
}

void Evaluator::visitInteger(Integer &integer)
{
    evalStack.push(std::make_shared<IntegerObject>(integer.value));
}

void Evaluator::visitBoolean(Boolean &boolean)
{
    evalStack.push(std::make_shared<BooleanObject>(boolean.value));
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
        auto right = evalStack.top();
        evalStack.pop();
        if(expression.token->type == Token::BANG)
        {
            evalStack.push(right->evalBangPrefixExpression());
        }
        else if (expression.token->type == Token::MINUS)
        {
            evalStack.push(right->evalMinusPrefixExpression());
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
    if(goingUp)
    {
        goingUp = false;
        auto rightEvaluated = evalStack.top();
        evalStack.pop();
        auto leftEvaluated = evalStack.top();
        evalStack.pop();

        if ((leftEvaluated->getType() == Object::Type::INTEGER) &&
            (rightEvaluated->getType() == Object::Type::INTEGER))
        {
            evalStack.push(evalIntegerInfixExpression(expression.token->type,
                    dynamic_cast<IntegerObject *>(leftEvaluated.get()),
                    dynamic_cast<IntegerObject *>(rightEvaluated.get())));
        }
        else if ((leftEvaluated->getType() == Object::Type::BOOLEAN) &&
                (rightEvaluated->getType() == Object::Type::BOOLEAN))
        {
            evalStack.push(evalBooleanInfixExpression(expression.token->type,
                    dynamic_cast<BooleanObject *>(leftEvaluated.get()),
                    dynamic_cast<BooleanObject *>(rightEvaluated.get())));
        }
        else
        {
            evalStack.push(std::make_shared<NullObject>());
        }
    }
    else
    {
        visitStack.push(std::make_shared<InfixExpression>(expression));
        visitStack.push(std::make_shared<ControlToken>(""));
        visitStack.push(expression.right);
        visitStack.push(expression.left);
    }
}

void Evaluator::visitIfExpression(IfExpression &expression)
{
    if(goingUp)
    {
        auto result = evalStack.top();
        evalStack.pop();

        if (isTruthy(result))
        {
            visitStack.push(expression.consequence);
        }
        else
        {
            if (expression.alternative != nullptr)
            {
                visitStack.push(expression.alternative);
            }
            else
            {
                evalStack.push(std::make_shared<NullObject>());
            }
        }
    }
    else
    {
        visitStack.push(std::make_shared<IfExpression>(expression));
        visitStack.push(std::make_shared<ControlToken>(""));
        visitStack.push(expression.condition);
    }
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
    addStatements(statement.statements);
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

std::shared_ptr<Object>
Evaluator::evalIntegerInfixExpression(Token::TokenType op, IntegerObject *left, IntegerObject *right)
{
    if((left != nullptr) && (right != nullptr))
    {
        auto leftValue = left->getValue();
        auto rightValue = right->getValue();

        switch (op)
        {
            case Token::PLUS:
                return std::make_shared<IntegerObject>(leftValue + rightValue);
            case Token::MINUS:
                return std::make_shared<IntegerObject>(leftValue - rightValue);
            case Token::ASTERISK:
                return std::make_shared<IntegerObject>(leftValue * rightValue);
            case Token::SLASH:
                return std::make_shared<IntegerObject>(leftValue / rightValue);
            case Token::LT:
                return std::make_shared<BooleanObject>(leftValue < rightValue);
            case Token::GT:
                return std::make_shared<BooleanObject>(leftValue > rightValue);
            case Token::EQ:
                return std::make_shared<BooleanObject>(leftValue == rightValue);
            case Token::NEQ:
                return std::make_shared<BooleanObject>(leftValue != rightValue);
            default:
                break;
        }
    }
    return std::make_shared<NullObject>();
}

std::shared_ptr<Object>
Evaluator::evalBooleanInfixExpression(Token::TokenType op, BooleanObject *left, BooleanObject *right)
{
    if((left != nullptr) && (right != nullptr))
    {
        auto leftValue = left->getValue();
        auto rightValue = right->getValue();

        switch (op)
        {
            case Token::EQ:
                return std::make_shared<BooleanObject>(leftValue == rightValue);
            case Token::NEQ:
                return std::make_shared<BooleanObject>(leftValue != rightValue);
            default:
                break;
        }
    }
    return std::make_shared<NullObject>();
}

bool Evaluator::isTruthy(const std::shared_ptr<Object>& object)
{
    switch (object->getType())
    {
        case Object::BOOLEAN:
            return dynamic_cast<BooleanObject *>(object.get())->getValue();
        case Object::NULLOBJECT:
            return false;
        default:
            return true;
    }
}



/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include "AstPrinter.h"
#include <utility>

ControlToken::ControlToken(std::string string) : _string(std::move(string)) {}

std::string ControlToken::string()
{
    return _string;
}

std::shared_ptr<Object> ControlToken::eval()
{
    return nullptr;
}

void ControlToken::accept(AstVisitor &visitor)
{
    visitor.visitControlToken(*this);
}


AstPrinter::AstPrinter() : output ("") {}

std::string AstPrinter::printCode(const std::shared_ptr <Node>& startNode)
{
    visitStack.push(startNode);

    // Visit all nodes in the visitList - nodes are added and removed dynamically
    while (!visitStack.empty())
    {
        std::shared_ptr<Node> node = visitStack.top();
        visitStack.pop();
        node->accept(*this);
    }

    return output;
}

void AstPrinter::visitIdentifier(Identifier &identifier)
{
    output.append(*identifier.value);
}

void AstPrinter::visitInteger(Integer &integer)
{
    output.append(std::to_string(integer.value));
}

void AstPrinter::visitBoolean(Boolean &boolean)
{
    output.append(boolean.value ? "true" : "false");
}

void AstPrinter::visitFunction(Function &function)
{
    output.append("fn(");
    visitStack.push(function.body);
    visitStack.push(std::make_shared<ControlToken>(") "));
    for(auto parameter = function.parameters.rbegin(); parameter != function.parameters.rend(); parameter++)
    {
        if (parameter != function.parameters.rbegin())
        {
            visitStack.push(std::make_shared<ControlToken>(", "));
        }
        visitStack.push(*parameter);
    }
}

void AstPrinter::visitCallExpression(CallExpression &expression)
{
    visitStack.push(std::make_shared<ControlToken>(")"));
    for(auto arg = expression.arguments.rbegin(); arg != expression.arguments.rend(); arg++)
    {
        if (arg != expression.arguments.rbegin())
        {
            visitStack.push(std::make_shared<ControlToken>(", "));
        }
        visitStack.push(*arg);
    }
    visitStack.push(std::make_shared<ControlToken>("("));
    visitStack.push(expression.function);
}

void AstPrinter::visitPrefixExpression(PrefixExpression &expression)
{
    output.append(expression.op);
    visitStack.push(expression.right);
}

void AstPrinter::visitInfixExpression(InfixExpression &expression)
{
    visitStack.push(expression.right);
    visitStack.push(std::make_shared<ControlToken>(expression.op));
    visitStack.push(expression.left);
}

void AstPrinter::visitIfExpression(IfExpression &expression)
{
    output.append("if (");
    if(expression.alternative != nullptr)
    {
        visitStack.push(expression.alternative);
        visitStack.push(std::make_shared<ControlToken>(" else "));
    }
    visitStack.push(expression.consequence);
    visitStack.push(std::make_shared<ControlToken>(") "));
    visitStack.push(expression.condition);
}

void AstPrinter::visitLetStatement(LetStatement &statement)
{
    output.append("let ");
    output.append(statement.identifier->string());
    output.append("=");
    visitStack.push(std::make_shared<ControlToken>(";"));
    visitStack.push(statement.expression);
}

void AstPrinter::visitExpressionStatement(ExpressionStatement &statement)
{
    visitStack.push(std::make_shared<ControlToken>(";"));
    visitStack.push(statement.expression);
}

void AstPrinter::visitReturnStatement(ReturnStatement &statement)
{
    output.append("return ");
    visitStack.push(std::make_shared<ControlToken>(";"));
    visitStack.push(statement.expression);
}

void AstPrinter::visitBlockStatement(BlockStatement &statement)
{
    output.append("{");
    visitStack.push(std::make_shared<ControlToken>("}"));
    addStatements(statement.statements);
}

void AstPrinter::visitProgram(Program &program)
{
    addStatements(program.statements);
}

void AstPrinter::visitControlToken(ControlToken &controlToken)
{
    if((controlToken.string() == "}") && (visitStack.top()->string() == ";"))
    {
        // This is a special case where we end a block statement and don't
        // want to print the ";". Thus, it is simply removed.
        visitStack.pop();
    }
    output.append(controlToken.string());
}

void AstPrinter::addStatements(std::vector<std::shared_ptr<Statement>> statements)
{
    // Push all statements to be visited in the reverse order
    for(auto statement = statements.rbegin(); statement != statements.rend(); statement++)
    {
        visitStack.push(*statement);
    }
}


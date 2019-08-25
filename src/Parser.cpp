/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Parser.h"

Parser::Parser(Lexer &lexer) : lexer(lexer)
{
    curToken = lexer.nextToken();
    peekToken = lexer.nextToken();
}

void Parser::nextToken()
{
    curToken = std::move(peekToken);
    peekToken = lexer.nextToken();
}

bool Parser::peekTokenIs(const Token::TokenType &type) const
{
    return peekToken->type == type;
}

bool Parser::expectPeek(Token::TokenType type)
{
    if (peekTokenIs(type))
    {
        nextToken();
        return true;
    }
    return false;
}

std::unique_ptr<Program> Parser::parseProgram()
{
    std::unique_ptr<Program> program = std::make_unique<Program>();

    // ... Parse the program ...
    while (curToken->type != Token::ENDOFFILE)
    {
        auto statement = parseStatement();
        if (statement != nullptr)
        {
            program->addStatement(std::move(statement));
        }
        nextToken();
    }

    return program;
}

std::unique_ptr<Statement> Parser::parseStatement()
{
    std::unique_ptr<Statement> statement;

    switch (curToken->type)
    {
        case Token::LET:
            statement = parseLetStatement();
            break;

        default:
            statement = nullptr;
            break;
    }
    return statement;
}

std::unique_ptr<Statement> Parser::parseLetStatement()
{
    auto letStatement = std::make_unique<LetStatement>(std::move(curToken));
    if (!expectPeek(Token::IDENTIFIER))
    {
        return nullptr;
    }

    letStatement->identifier = std::make_unique<Identifier>(std::move(curToken));

    if (!expectPeek(Token::ASSIGN))
    {
        return nullptr;
    }

    return letStatement;
}


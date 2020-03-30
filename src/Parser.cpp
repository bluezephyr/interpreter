/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Exceptions.h"
#include "Parser.h"

Parser::Parser(Lexer &lexer) : lexer(lexer)
{
    curToken = lexer.nextToken();
    peekToken = lexer.nextToken();
}

bool Parser::currentTokenIs(const Token::TokenType &type) const
{
    return curToken->type == type;
}

bool Parser::peekTokenIs(const Token::TokenType &type) const
{
    return peekToken->type == type;
}

void Parser::nextToken()
{
    curToken = std::move(peekToken);
    peekToken = lexer.nextToken();
}

void Parser::nextTokenIfType(Token::TokenType type)
{
    if (peekTokenIs(type))
    {
        nextToken();
    }
    else
    {
        std::string message("Expected " + Token::getTypeString(type) + " token. Got " +
                                    Token::getTypeString(peekToken->type) + " token (" +
                                    *(peekToken->literal) + ")");
        errors.emplace_back(message);
        throw(WrongTokenException());
    }
}

std::unique_ptr<Program> Parser::parseProgram()
{
    std::unique_ptr<Program> program = std::make_unique<Program>();

    // ... Parse the program ...
    while (!currentTokenIs(Token::ENDOFFILE))
    {
        try
        {
            program->addStatement(parseStatement());

            // Consume semicolon
            if (curToken->type == Token::SEMICOLON)
            {
                nextToken();
            }
        }
        catch (InvalidStatementException&)
        {
            nextToken();
        }
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

        case Token::RETURN:
            statement = parseReturnStatement();
            break;

        default:
            throw InvalidStatementException();
    }
    return statement;
}

std::unique_ptr<Statement> Parser::parseLetStatement()
{
    auto statement = std::make_unique<LetStatement>(std::move(curToken));

    try
    {
        nextTokenIfType(Token::IDENTIFIER);
        statement->identifier = std::make_unique<Identifier>(std::move(curToken));
        nextTokenIfType(Token::ASSIGN);
    }
    catch (WrongTokenException&)
    {
        // TODO: Consume the rest of the statement - for now, just get next token and
        //       let the while loop below handle it.
        nextToken();
    }

    // TODO: Implement expression parsing.
    //       For now consume until the semicolon.
    while ((curToken->type != Token::SEMICOLON) && (curToken->type != Token::ENDOFFILE))
    {
        nextToken();
    }

    return statement;
}

std::unique_ptr<Statement> Parser::parseReturnStatement()
{
    auto statement = std::make_unique<ReturnStatement>(std::move(curToken));
    nextToken();

    // TODO: Implement expression parsing.
    //       For now consume until the semicolon.
    while ((curToken->type != Token::SEMICOLON) && (curToken->type != Token::ENDOFFILE))
    {
        nextToken();
    }

    return statement;
}


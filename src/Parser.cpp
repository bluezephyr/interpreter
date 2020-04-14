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
    prefixParseFunctionMap[Token::IDENTIFIER] = std::bind(&Parser::parseIdentifier, this);
    prefixParseFunctionMap[Token::INT] = std::bind(&Parser::parseInteger, this);
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
    if (currentTokenIs(type))
    {
        nextToken();
    }
    else
    {
        std::string message("Expected " + Token::getTypeString(type) + " token. Got " +
                                    Token::getTypeString(curToken->type) + " token (" +
                                    *(curToken->literal) + ")");
        errors.emplace_back(message);
        throw WrongTokenException();
    }
}

std::shared_ptr<Program> Parser::parseProgram()
{
    std::shared_ptr<Program> program = std::make_shared<Program>();

    // ... Parse the program ...
    try
    {
        while (!currentTokenIs(Token::ENDOFFILE))
        {
            try
            {
                program->addStatement(parseStatement());
            }
            catch (ParserException &)
            {
                // TODO: Consume the rest of the statement - for now, just get next token and
                //       let the while loop below handle it.
                while (curToken->type != Token::SEMICOLON)
                {
                    nextToken();
                }
            }
            consumeSemicolon();
        }
    }
    catch (EndOfFileException&)
    {
        errors.emplace_back("Unexpected EOF");
    }

    return program;
}

void Parser::consumeSemicolon()
{
    // Consume semicolon if present
    if (curToken->type == Token::SEMICOLON)
    {
        nextToken();
    }
}

std::shared_ptr<Statement> Parser::parseStatement()
{
    std::shared_ptr<Statement> statement;

    switch (curToken->type)
    {
        case Token::LET:
            statement = parseLetStatement();
            break;

        case Token::RETURN:
            statement = parseReturnStatement();
            break;

        default:
            statement = parseExpressionStatement();
            break;
    }
    return statement;
}

std::shared_ptr<Statement> Parser::parseLetStatement()
{
    auto statement = std::make_shared<LetStatement>(std::move(curToken));
    nextToken();

    statement->identifier = parseIdentifier();
    nextTokenIfType(Token::ASSIGN);
    statement->expression = parseExpression(Precedence::LOWEST);
    return statement;
}

std::shared_ptr<Statement> Parser::parseReturnStatement()
{
    auto statement = std::make_shared<ReturnStatement>(std::move(curToken));
    nextToken();
    statement->expression = parseExpression(Precedence::LOWEST);
    return statement;
}

std::shared_ptr<Statement> Parser::parseExpressionStatement()
{
    auto statement = std::make_shared<ExpressionStatement>();
    statement->expression = parseExpression(Precedence::LOWEST);
    return statement;
}

std::shared_ptr<Identifier> Parser::parseIdentifier()
{
    if(currentTokenIs(Token::IDENTIFIER))
    {
        auto identifier = std::make_shared<Identifier>(std::move(curToken));
        nextToken();
        return identifier;
    }
    else
    {
        std::string message("Expected " + Token::getTypeString(Token::IDENTIFIER) + " token. Got " +
                            Token::getTypeString(curToken->type) + " token (" +
                            *(curToken->literal) + ")");
        errors.emplace_back(message);
        throw WrongTokenException();
    }
}

std::shared_ptr<Integer> Parser::parseInteger()
{
    if (currentTokenIs(Token::INT))
    {
        auto integer = std::make_shared<Integer>(std::move(curToken));
        integer->value = std::stol(*integer->token->literal);
        nextToken();
        return integer;
    }
    else
    {
        std::string message("Expected " + Token::getTypeString(Token::INT) + " token. Got " +
                            Token::getTypeString(curToken->type) + " token (" +
                            *(curToken->literal) + ")");
        errors.emplace_back(message);
        throw WrongTokenException();
    }
}

std::shared_ptr<Expression> Parser::parseExpression(Precedence precedence)
{
    std::shared_ptr<Expression> leftExpression = nullptr;

    auto prefixParseFunction = getPrefixParseFunction(curToken->type);
    if (prefixParseFunction != nullptr)
    {
        leftExpression = prefixParseFunction();
    }
    else
    {
        // TODO: For now...
        nextToken();
    }

    return leftExpression;
}

PrefixParseFunction Parser::getPrefixParseFunction(Token::TokenType type)
{
    return prefixParseFunctionMap[type];
}


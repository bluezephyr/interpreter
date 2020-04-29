/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Exceptions.h"
#include "Parser.h"

#include <utility>

Parser::Parser(Lexer &lexer) : lexer(lexer)
{
    prefixParseFunctionMap[Token::IDENTIFIER] = &Parser::parseIdentifier;
    prefixParseFunctionMap[Token::INT] = &Parser::parseInteger;
    prefixParseFunctionMap[Token::BANG] = &Parser::parsePrefixExpression;
    prefixParseFunctionMap[Token::MINUS] = &Parser::parsePrefixExpression;
    prefixParseFunctionMap[Token::LPAREN] = &Parser::parseGroupedExpression;
    prefixParseFunctionMap[Token::TRUE] = &Parser::parseBoolean;
    prefixParseFunctionMap[Token::FALSE] = &Parser::parseBoolean;
    prefixParseFunctionMap[Token::IF] = &Parser::parseIfExpression;
    prefixParseFunctionMap[Token::FUNCTION] = &Parser::parseFunction;
    infixParseFunctionMap[Token::PLUS] = &Parser::parseInfixExpression;
    infixParseFunctionMap[Token::MINUS] = &Parser::parseInfixExpression;
    infixParseFunctionMap[Token::ASTERISK] = &Parser::parseInfixExpression;
    infixParseFunctionMap[Token::SLASH] = &Parser::parseInfixExpression;
    infixParseFunctionMap[Token::GT] = &Parser::parseInfixExpression;
    infixParseFunctionMap[Token::LT] = &Parser::parseInfixExpression;
    infixParseFunctionMap[Token::EQ] = &Parser::parseInfixExpression;
    infixParseFunctionMap[Token::NEQ] = &Parser::parseInfixExpression;
    infixParseFunctionMap[Token::LPAREN] = &Parser::parseCallExpression;
    precedenceMap[Token::EQ] = Precedence::EQUALS;
    precedenceMap[Token::NEQ] = Precedence::EQUALS;
    precedenceMap[Token::LT] = Precedence::LESSGREATER;
    precedenceMap[Token::GT] = Precedence::LESSGREATER;
    precedenceMap[Token::PLUS] = Precedence::SUM;
    precedenceMap[Token::MINUS] = Precedence::SUM;
    precedenceMap[Token::SLASH] = Precedence::PRODUCT;
    precedenceMap[Token::ASTERISK] = Precedence::PRODUCT;
    precedenceMap[Token::LPAREN] = Precedence::CALL;
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
    if (peekToken != nullptr)
    {
        curToken = std::move(peekToken);
        peekToken = lexer.nextToken();
    }
    else
    {
        throw NoMoreTokensException();
    }
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

Precedence Parser::getPrecedence(Token::TokenType tokenType)
{
    auto precedence = precedenceMap.find(tokenType);

    if (precedence != precedenceMap.end())
    {
        return precedence->second;
    }
    else
    {
        return Precedence::LOWEST;
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
                // Consume the rest of the statement and continue parsing after that
                while (curToken->type != Token::SEMICOLON)
                {
                    nextToken();
                }
                consumeSemicolon();
            }
        }
    }
    catch (NoMoreTokensException&)
    {
        errors.emplace_back("Expected more tokens, but none present");
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
    consumeSemicolon();
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

std::shared_ptr<Statement> Parser::parseBlockStatement()
{
    auto block = std::make_shared<BlockStatement>();

    try
    {
        while (!currentTokenIs(Token::RBRACE))
        {
            try
            {
                block->statements.emplace_back(parseStatement());
            }
            catch (ParserException &)
            {
                // Consume the rest of the statement and continue parsing after that
                while (curToken->type != Token::SEMICOLON)
                {
                    nextToken();
                }
            }
        }
    }
    catch (NoMoreTokensException&)
    {
        errors.emplace_back("Expected more tokens, but none present");
    }

    nextToken();
    return block;
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

std::shared_ptr<Boolean> Parser::parseBoolean()
{
    std::shared_ptr<Boolean> boolean = std::make_shared<Boolean>(
            std::move(curToken),
            currentTokenIs(Token::TRUE));
    nextToken();
    return boolean;
}

// fn ( [ <parameter 1>, <parameter 2>, ... ] ) { <consequence> } [ else { <alternative> } ]
std::shared_ptr<Function> Parser::parseFunction()
{
    auto function = std::make_shared<Function>(std::move(curToken));
    nextToken();
    nextTokenIfType(Token::LPAREN);
    function->parameters = parseFunctionParameters();
    nextTokenIfType(Token::RPAREN);
    nextTokenIfType(Token::LBRACE);
    function->body = parseBlockStatement();

    return function;
}

std::vector<std::shared_ptr<Identifier>> Parser::parseFunctionParameters()
{
    std::vector<std::shared_ptr<Identifier>> parameters;

    while(!currentTokenIs(Token::RPAREN))
    {
        parameters.push_back(parseIdentifier());
        if (currentTokenIs(Token::RPAREN))
        {
            break;
        }

        nextTokenIfType(Token::COMMA);
    }
    return std::move(parameters);
}

std::shared_ptr<Expression> Parser::parsePrefixExpression()
{
    auto expression = std::make_shared<PrefixExpression>(std::move(curToken));
    expression->op = std::string(*expression->token->literal);
    nextToken();
    expression->right = parseExpression(Precedence::PREFIX);
    return expression;
}

std::shared_ptr<Expression> Parser::parseGroupedExpression()
{
    nextToken();
    auto expression = parseExpression(Precedence::LOWEST);
    if (currentTokenIs(Token::RPAREN))
    {
        nextToken();
    }
    else
    {
        std::string message("Expected " + Token::getTypeString(Token::RPAREN) + " token. Got " +
                            Token::getTypeString(curToken->type) + " token (" +
                            *(curToken->literal) + ")");
        errors.emplace_back(message);
        throw WrongTokenException();
    }
    return expression;
}

std::shared_ptr<Expression> Parser::parseInfixExpression(std::shared_ptr<Expression> left)
{
    auto expression = std::make_shared<InfixExpression>(std::move(curToken));
    expression->left = std::move(left);
    expression->op = std::string(*expression->token->literal);
    nextToken();
    expression->right = parseExpression(getPrecedence(expression->token->type));
    return expression;
}

std::shared_ptr<Expression> Parser::parseExpression(Precedence precedence)
{
    auto prefixParseFunction = getPrefixParseFunction(curToken->type);
    std::shared_ptr<Expression> leftExpression = prefixParseFunction(this);

    while (precedence < getPrecedence(curToken->type))
    {
        auto infixParseFunction = getInfixParseFunction(curToken->type);
        leftExpression = infixParseFunction(this, leftExpression);
    }

    return leftExpression;
}

// if ( <condition> ) { <consequence> } [ else { <alternative> } ]
std::shared_ptr<Expression> Parser::parseIfExpression()
{
    auto expression = std::make_shared<IfExpression>(std::move(curToken));
    nextToken();
    nextTokenIfType(Token::LPAREN);
    expression->condition = parseExpression(Precedence::LOWEST);
    nextTokenIfType(Token::RPAREN);
    nextTokenIfType(Token::LBRACE);
    expression->consequence = parseBlockStatement();
    if(currentTokenIs(Token::ELSE))
    {
        nextToken();
        nextTokenIfType(Token::LBRACE);
        expression->alternative = parseBlockStatement();
    }
    return expression;
}

std::shared_ptr<Expression> Parser::parseCallExpression(std::shared_ptr<Expression> function)
{
    auto callExpression = std::make_shared<CallExpression>(std::move(curToken));
    callExpression->function = std::move(function);
    nextToken();
    callExpression->arguments = parseCallArguments();
    return callExpression;
}

std::vector<std::shared_ptr<Expression>> Parser::parseCallArguments()
{
    std::vector<std::shared_ptr<Expression>> arguments;

    while(!currentTokenIs(Token::RPAREN))
    {
        arguments.push_back(parseExpression(Precedence::LOWEST));
        if (currentTokenIs(Token::RPAREN))
        {
            break;
        }

        nextTokenIfType(Token::COMMA);
    }
    nextTokenIfType(Token::RPAREN);
    return std::move(arguments);
}

PrefixParseFunction Parser::getPrefixParseFunction(Token::TokenType type)
{
    auto function = prefixParseFunctionMap.find(type);
    if (function != prefixParseFunctionMap.end())
    {
        return function->second;
    }
    else
    {
        std::string message("No prefix parse function for " + Token::getTypeString(curToken->type) + " found");
        errors.emplace_back(message);
        throw PrefixParseError();
    }
}

InfixParseFunction Parser::getInfixParseFunction(Token::TokenType type)
{
    auto function = infixParseFunctionMap.find(type);
    if (function != infixParseFunctionMap.end())
    {
        return infixParseFunctionMap[type];
    }
    else
    {
        std::string message("No infix parse function for " + Token::getTypeString(curToken->type) + " found");
        errors.emplace_back(message);
        throw InfixParseError();
    }
}


/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

#include <memory>
#include <functional>
#include "Ast.h"
#include "Lexer.h"

enum class Precedence
{
    LOWEST,
    EQUALS,
    LESSGREATER,
    SUM,
    PRODUCT,
    PREFIX,
    CALL
};

class Parser;
typedef std::function<std::shared_ptr<Expression>(Parser*)> PrefixParseFunction;
typedef std::function<std::shared_ptr<Expression>(Parser*, std::shared_ptr<Expression>)> InfixParseFunction;

class Parser
{
public:
    explicit Parser(Lexer &lexer);
    std::shared_ptr<Program> parseProgram();
    std::vector<std::string> errors;

private:
    Lexer& lexer;
    std::unique_ptr<Token> curToken;
    std::unique_ptr<Token> peekToken;
    std::unordered_map<Token::TokenType, PrefixParseFunction> prefixParseFunctionMap;
    std::unordered_map<Token::TokenType, InfixParseFunction> infixParseFunctionMap;
    std::unordered_map<Token::TokenType, Precedence> precedenceMap;

    bool currentTokenIs(const Token::TokenType &) const;
    bool peekTokenIs(const Token::TokenType &) const;
    void nextToken();
    void nextTokenIfType(Token::TokenType);
    Precedence getPrecedence(Token::TokenType);
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<Statement> parseLetStatement();
    std::shared_ptr<Statement> parseReturnStatement();
    std::shared_ptr<Statement> parseExpressionStatement();
    std::shared_ptr<Statement> parseBlockStatement();
    std::shared_ptr<Identifier> parseIdentifier();
    std::shared_ptr<Integer> parseInteger();
    std::shared_ptr<Boolean> parseBoolean();
    std::shared_ptr<Expression> parsePrefixExpression();
    std::shared_ptr<Expression> parseGroupedExpression();
    std::shared_ptr<Expression> parseInfixExpression(std::shared_ptr<Expression>);
    std::shared_ptr<Expression> parseExpression(Precedence);
    std::shared_ptr<Expression> parseIfExpression();
    void consumeSemicolon();

    PrefixParseFunction getPrefixParseFunction(Token::TokenType);
    InfixParseFunction getInfixParseFunction(Token::TokenType);
};

#endif //INTERPRETER_PARSER_H

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
typedef std::function<std::shared_ptr<Expression>(void)> PrefixParseFunction;

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

    bool currentTokenIs(const Token::TokenType &type) const;
    bool peekTokenIs(const Token::TokenType &type) const;
    void nextToken();
    void nextTokenIfType(Token::TokenType type);
    std::shared_ptr<Statement> parseStatement();
    std::shared_ptr<Statement> parseLetStatement();
    std::shared_ptr<Statement> parseReturnStatement();
    std::shared_ptr<Statement> parseExpressionStatement();
    std::shared_ptr<Identifier> parseIdentifier();
    std::shared_ptr<Integer> parseInteger();
    std::shared_ptr<Expression> parseExpression(Precedence precedence);
    void consumeSemicolon();

    PrefixParseFunction getPrefixParseFunction(Token::TokenType type);
};

#endif //INTERPRETER_PARSER_H

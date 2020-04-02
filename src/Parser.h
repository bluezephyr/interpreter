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
#include "ast.h"
#include "Lexer.h"

class Parser
{
public:
    explicit Parser(Lexer &lexer);
    std::unique_ptr<Program> parseProgram();
    std::vector<std::string> errors;

private:
    Lexer& lexer;
    std::unique_ptr<Token> curToken;
    std::unique_ptr<Token> peekToken;

    bool currentTokenIs(const Token::TokenType &type) const;
    bool peekTokenIs(const Token::TokenType &type) const;
    void nextToken();
    void nextTokenIfType(Token::TokenType type);
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseLetStatement();
    std::unique_ptr<Statement> parseReturnStatement();
    std::shared_ptr<Identifier> parseIdentifier();

    void parseExpression();
};

#endif //INTERPRETER_PARSER_H

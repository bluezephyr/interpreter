/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef INTERPRETER_PARSER_H
#define INTERPRETER_PARSER_H

#include <memory>
#include "Lexer.h"
#include "ast.h"

class Parser
{
public:
    explicit Parser(Lexer &lexer);
    std::unique_ptr<Program> parseProgram();

private:
    Lexer lexer;
    std::unique_ptr<Token> curToken;
    std::unique_ptr<Token> peekToken;
    void nextToken();
    bool peekTokenIs(const Token::TokenType &type) const;
    bool expectPeek(Token::TokenType type);
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseLetStatement();

};

#endif //INTERPRETER_PARSER_H

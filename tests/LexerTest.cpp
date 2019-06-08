/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <Lexer.h>
#include <Token.h>
#include "CppUTest/TestHarness.h"

TEST_GROUP(LexerTest)
{
    Lexer *lexer;
    Token *token;

    void setup()
    {
    }

    void teardown()
    {
        delete(lexer);
        delete(token);
    }

    void checkSingleCharToken(char c, Token::TokenType type)
    {
        char string[2];

        string[0] = c;
        string[1] = 0;

        lexer = new Lexer(string);
        token = lexer->nextToken();

        CHECK_EQUAL(type, token->type);
        CHECK_EQUAL(std::string(1, c), *(token->literal));
    }
};

TEST(LexerTest, nextTokenEOF)
{
    lexer = new Lexer("");
    token = lexer->nextToken();

    CHECK_EQUAL(Token::ENDOFFILE, token->type);
    CHECK(token->literal->empty());
}

TEST(LexerTest, nextTokenAssign)
{
    checkSingleCharToken('=', Token::ASSIGN);
}

TEST(LexerTest, nextTokenPlus)
{
    checkSingleCharToken('+', Token::PLUS);
}

TEST(LexerTest, nextTokenLeftParenthesis)
{
    checkSingleCharToken('(', Token::LPAREN);
}

TEST(LexerTest, nextTokenRightParenthesis)
{
    checkSingleCharToken(')', Token::RPAREN);
}

TEST(LexerTest, nextTokenLeftBracket)
{
    checkSingleCharToken('[', Token::LBRACE);
}

TEST(LexerTest, nextTokenRightBracket)
{
    checkSingleCharToken(']', Token::RBRACE);
}

TEST(LexerTest, nextTokenComma)
{
    checkSingleCharToken(',', Token::COMMA);
}

TEST(LexerTest, nextTokenSemicolon)
{
    checkSingleCharToken(';', Token::SEMICOLON);
}

/*
 * Tests to write:
 * -
 */

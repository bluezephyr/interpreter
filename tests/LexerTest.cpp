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
    }

    void checkSingleCharToken(char c, Token::TokenType type)
    {
        token = lexer->nextToken();
        CHECK_EQUAL(type, token->type);
        CHECK_EQUAL(std::string(1, c), *(token->literal));
        delete(token);
    }
};

TEST(LexerTest, nextTokenEOF)
{
    lexer = new Lexer("");
    token = lexer->nextToken();

    CHECK_EQUAL(Token::ENDOFFILE, token->type);
    CHECK(token->literal->empty());
    delete(token);
}

TEST(LexerTest, nextTokenAssign)
{
    lexer = new Lexer("=");
    checkSingleCharToken('=', Token::ASSIGN);
}

TEST(LexerTest, nextTokenPlus)
{
    lexer = new Lexer("+");
    checkSingleCharToken('+', Token::PLUS);
}

TEST(LexerTest, nextTokenLeftParenthesis)
{
    lexer = new Lexer("(");
    checkSingleCharToken('(', Token::LPAREN);
}

TEST(LexerTest, nextTokenRightParenthesis)
{
    lexer = new Lexer(")");
    checkSingleCharToken(')', Token::RPAREN);
}

TEST(LexerTest, nextTokenLeftBracket)
{
    lexer = new Lexer("[");
    checkSingleCharToken('[', Token::LBRACE);
}

TEST(LexerTest, nextTokenRightBracket)
{
    lexer = new Lexer("]");
    checkSingleCharToken(']', Token::RBRACE);
}

TEST(LexerTest, nextTokenComma)
{
    lexer = new Lexer(",");
    checkSingleCharToken(',', Token::COMMA);
}

TEST(LexerTest, nextTokenSemicolon)
{
    lexer = new Lexer(";");
    checkSingleCharToken(';', Token::SEMICOLON);
}

TEST(LexerTest, readTokenSequence)
{
    lexer = new Lexer("=+()[],;");

    checkSingleCharToken('=', Token::ASSIGN);
    checkSingleCharToken('+', Token::PLUS);
    checkSingleCharToken('(', Token::LPAREN);
    checkSingleCharToken(')', Token::RPAREN);
    checkSingleCharToken('[', Token::LBRACE);
    checkSingleCharToken(']', Token::RBRACE);
    checkSingleCharToken(',', Token::COMMA);
    checkSingleCharToken(';', Token::SEMICOLON);
}

TEST(LexerTest, readTokenLet)
{
    lexer = new Lexer("let");
    token = lexer->nextToken();

    CHECK_EQUAL(Token::LET, token->type);
    CHECK_EQUAL(std::string("let"), *(token->literal));
    delete(token);
}

TEST(LexerTest, readTokenFn)
{
    lexer = new Lexer("fn");
    token = lexer->nextToken();

    CHECK_EQUAL(Token::FUNCTION, token->type);
    CHECK_EQUAL(std::string("fn"), *(token->literal));
    delete(token);
}

/*
 * Tests to write:
 * -
 */

/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <Token.h>
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(TokenTest)
{
    void setup() override {}
    void teardown() override {}
};

TEST(TokenTest, lookUpToken)
{
    CHECK_EQUAL(Token::LET, Token("let").type);
    CHECK_EQUAL(Token::FUNCTION, Token("fn").type);
    CHECK_EQUAL(Token::TRUE, Token("true").type);
    CHECK_EQUAL(Token::FALSE, Token("false").type);
    CHECK_EQUAL(Token::IF, Token("if").type);
    CHECK_EQUAL(Token::ELSE, Token("else").type);
    CHECK_EQUAL(Token::RETURN, Token("return").type);
}

TEST(TokenTest, getTypeString)
{
    CHECK_EQUAL("LET", Token("let").getTypeString());
    CHECK_EQUAL("FN", Token("fn").getTypeString());
    CHECK_EQUAL("TRUE", Token("true").getTypeString());
    CHECK_EQUAL("FALSE", Token("false").getTypeString());
    CHECK_EQUAL("IF", Token("if").getTypeString());
    CHECK_EQUAL("ELSE", Token("else").getTypeString());
    CHECK_EQUAL("RETURN", Token("return").getTypeString());
    CHECK_EQUAL("ILLEGAL", Token(Token::ILLEGAL, "").getTypeString());
    CHECK_EQUAL("EOF", Token(Token::ENDOFFILE, "").getTypeString());
    CHECK_EQUAL("IDENTIFIER", Token(Token::IDENTIFIER, "").getTypeString());
    CHECK_EQUAL("INT", Token(Token::INT, "").getTypeString());
    CHECK_EQUAL("ASSIGN", Token(Token::ASSIGN, "").getTypeString());
    CHECK_EQUAL("PLUS", Token(Token::PLUS, "").getTypeString());
    CHECK_EQUAL("MINUS", Token(Token::MINUS, "").getTypeString());
    CHECK_EQUAL("BANG", Token(Token::BANG, "").getTypeString());
    CHECK_EQUAL("ASTERISK", Token(Token::ASTERISK, "").getTypeString());
    CHECK_EQUAL("SLASH", Token(Token::SLASH, "").getTypeString());
    CHECK_EQUAL("LT", Token(Token::LT, "").getTypeString());
    CHECK_EQUAL("GT", Token(Token::GT, "").getTypeString());
    CHECK_EQUAL("EQ", Token(Token::EQ, "").getTypeString());
    CHECK_EQUAL("NEQ", Token(Token::NEQ, "").getTypeString());
    CHECK_EQUAL("COMMA", Token(Token::COMMA, "").getTypeString());
    CHECK_EQUAL("SEMICOLON", Token(Token::SEMICOLON, "").getTypeString());
    CHECK_EQUAL("LPAREN", Token(Token::LPAREN, "").getTypeString());
    CHECK_EQUAL("RPAREN", Token(Token::RPAREN, "").getTypeString());
    CHECK_EQUAL("LBRACE", Token(Token::LBRACE, "").getTypeString());
    CHECK_EQUAL("RBRACE", Token(Token::RBRACE, "").getTypeString());
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

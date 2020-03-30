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
    CHECK_EQUAL("LET", Token::getTypeString(Token::LET));
    CHECK_EQUAL("FN", Token::getTypeString(Token::FUNCTION));
    CHECK_EQUAL("TRUE", Token::getTypeString(Token::TRUE));
    CHECK_EQUAL("FALSE", Token::getTypeString(Token::FALSE));
    CHECK_EQUAL("IF", Token::getTypeString(Token::IF));
    CHECK_EQUAL("ELSE", Token::getTypeString(Token::ELSE));
    CHECK_EQUAL("RETURN", Token::getTypeString(Token::RETURN));
    CHECK_EQUAL("ILLEGAL", Token::getTypeString(Token::ILLEGAL));
    CHECK_EQUAL("EOF", Token::getTypeString(Token::ENDOFFILE));
    CHECK_EQUAL("IDENTIFIER", Token::getTypeString(Token::IDENTIFIER));
    CHECK_EQUAL("INT", Token::getTypeString(Token::INT));
    CHECK_EQUAL("ASSIGN", Token::getTypeString(Token::ASSIGN));
    CHECK_EQUAL("PLUS", Token::getTypeString(Token::PLUS));
    CHECK_EQUAL("MINUS", Token::getTypeString(Token::MINUS));
    CHECK_EQUAL("BANG", Token::getTypeString(Token::BANG));
    CHECK_EQUAL("ASTERISK", Token::getTypeString(Token::ASTERISK));
    CHECK_EQUAL("SLASH", Token::getTypeString(Token::SLASH));
    CHECK_EQUAL("LT", Token::getTypeString(Token::LT));
    CHECK_EQUAL("GT", Token::getTypeString(Token::GT));
    CHECK_EQUAL("EQ", Token::getTypeString(Token::EQ));
    CHECK_EQUAL("NEQ", Token::getTypeString(Token::NEQ));
    CHECK_EQUAL("COMMA", Token::getTypeString(Token::COMMA));
    CHECK_EQUAL("SEMICOLON", Token::getTypeString(Token::SEMICOLON));
    CHECK_EQUAL("LPAREN", Token::getTypeString(Token::LPAREN));
    CHECK_EQUAL("RPAREN", Token::getTypeString(Token::RPAREN));
    CHECK_EQUAL("LBRACE", Token::getTypeString(Token::LBRACE));
    CHECK_EQUAL("RBRACE", Token::getTypeString(Token::RBRACE));
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

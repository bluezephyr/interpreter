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

    void assertNextTokenSingleCharToken(Token::TokenType type, char c)
    {
        token = lexer->nextToken();
        CHECK_EQUAL(type, token->type);
        CHECK_EQUAL(std::string(1, c), *(token->literal));
        delete(token);
    }

    void assertNextTokenDoubleCharToken(Token::TokenType type, const std::string& literal)
    {
        token = lexer->nextToken();
        CHECK_EQUAL(type, token->type);
        CHECK_EQUAL(literal, *(token->literal));
        delete(token);
    }

    void assertNextToken(Token::TokenType type, const std::string& literal)
    {
        token = lexer->nextToken();
        CHECK_EQUAL(type, token->type);
        CHECK_EQUAL(literal, *(token->literal));
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

TEST(LexerTest, nextTokenIllegal)
{
    lexer = new Lexer("?");
    assertNextTokenSingleCharToken(Token::ILLEGAL, '?');
}

TEST(LexerTest, nextTokenAssign)
{
    lexer = new Lexer("=");
    assertNextTokenSingleCharToken(Token::ASSIGN, '=');
}

TEST(LexerTest, nextTokenPlus)
{
    lexer = new Lexer("+");
    assertNextTokenSingleCharToken(Token::PLUS, '+');
}

TEST(LexerTest, nextTokenMinus)
{
    lexer = new Lexer("-");
    assertNextTokenSingleCharToken(Token::MINUS, '-');
}

TEST(LexerTest, nextTokenBang)
{
    lexer = new Lexer("!");
    assertNextTokenSingleCharToken(Token::BANG, '!');
}

TEST(LexerTest, nextTokenAsterisk)
{
    lexer = new Lexer("*");
    assertNextTokenSingleCharToken(Token::ASTERISK, '*');
}

TEST(LexerTest, nextTokenSlash)
{
    lexer = new Lexer("/");
    assertNextTokenSingleCharToken(Token::SLASH, '/');
}

TEST(LexerTest, nextTokenLessThan)
{
    lexer = new Lexer("<");
    assertNextTokenSingleCharToken(Token::LT, '<');
}

TEST(LexerTest, nextTokenGreaterThan)
{
    lexer = new Lexer(">");
    assertNextTokenSingleCharToken(Token::GT, '>');
}

TEST(LexerTest, nextTokenLeftParenthesis)
{
    lexer = new Lexer("(");
    assertNextTokenSingleCharToken(Token::LPAREN, '(');
}

TEST(LexerTest, nextTokenRightParenthesis)
{
    lexer = new Lexer(")");
    assertNextTokenSingleCharToken(Token::RPAREN, ')');
}

TEST(LexerTest, nextTokenLeftBracket)
{
    lexer = new Lexer("[");
    assertNextTokenSingleCharToken(Token::LBRACE, '[');
}

TEST(LexerTest, nextTokenRightBracket)
{
    lexer = new Lexer("]");
    assertNextTokenSingleCharToken(Token::RBRACE, ']');
}

TEST(LexerTest, nextTokenComma)
{
    lexer = new Lexer(",");
    assertNextTokenSingleCharToken(Token::COMMA, ',');
}

TEST(LexerTest, nextTokenSemicolon)
{
    lexer = new Lexer(";");
    assertNextTokenSingleCharToken(Token::SEMICOLON, ';');
}

TEST(LexerTest, readSingleCharTokenSequence)
{
    lexer = new Lexer("=+-!*/<>,;()[]");

    assertNextTokenSingleCharToken(Token::ASSIGN, '=');
    assertNextTokenSingleCharToken(Token::PLUS, '+');
    assertNextTokenSingleCharToken(Token::MINUS, '-');
    assertNextTokenSingleCharToken(Token::BANG, '!');
    assertNextTokenSingleCharToken(Token::ASTERISK, '*');
    assertNextTokenSingleCharToken(Token::SLASH, '/');
    assertNextTokenSingleCharToken(Token::LT, '<');
    assertNextTokenSingleCharToken(Token::GT, '>');
    assertNextTokenSingleCharToken(Token::COMMA, ',');
    assertNextTokenSingleCharToken(Token::SEMICOLON, ';');
    assertNextTokenSingleCharToken(Token::LPAREN, '(');
    assertNextTokenSingleCharToken(Token::RPAREN, ')');
    assertNextTokenSingleCharToken(Token::LBRACE, '[');
    assertNextTokenSingleCharToken(Token::RBRACE, ']');
}

TEST(LexerTest, nextTokenEqual)
{
    lexer = new Lexer("==");
    assertNextTokenDoubleCharToken(Token::EQ, std::string("=="));
}

TEST(LexerTest, nextTokenNotEqual)
{
    lexer = new Lexer("!=");
    assertNextTokenDoubleCharToken(Token::NEQ, std::string("!="));
}

TEST(LexerTest, readTokenKeywordLet)
{
    lexer = new Lexer("let");
    assertNextToken(Token::LET, std::string("let"));
}

TEST(LexerTest, readTokenOneLetterIdentifier)
{
    lexer = new Lexer("i");
    assertNextToken(Token::IDENTIFIER, std::string("i"));
}

TEST(LexerTest, readTokenIdentifier)
{
    lexer = new Lexer("five");
    assertNextToken(Token::IDENTIFIER, std::string("five"));
}

TEST(LexerTest, readAssignmentTokens)
{
    lexer = new Lexer("let five\t =\
                       5;");

    assertNextToken(Token::LET, std::string("let"));
    assertNextToken(Token::IDENTIFIER, std::string("five"));
    assertNextToken(Token::ASSIGN, std::string("="));
    assertNextToken(Token::INT, std::string("5"));
    assertNextToken(Token::SEMICOLON, std::string(";"));
}

TEST(LexerTest, readTokenKeywordFn)
{
    lexer = new Lexer("fn");
    assertNextToken(Token::FUNCTION, std::string("fn"));
}

TEST(LexerTest, readTokenKeywordTrue)
{
    lexer = new Lexer("true");
    assertNextToken(Token::TRUE, std::string("true"));
}

TEST(LexerTest, readTokenKeywordFalse)
{
    lexer = new Lexer("false");
    assertNextToken(Token::FALSE, std::string("false"));
}

TEST(LexerTest, readTokenKeywordIf)
{
    lexer = new Lexer("if");
    assertNextToken(Token::IF, std::string("if"));
}

TEST(LexerTest, readTokenKeywordElse)
{
    lexer = new Lexer("else");
    assertNextToken(Token::ELSE, std::string("else"));
}

TEST(LexerTest, readTokenKeywordReturn)
{
    lexer = new Lexer("return");
    assertNextToken(Token::RETURN, std::string("return"));
}

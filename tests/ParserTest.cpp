/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <vector>
#include "Parser.h"
#include "ast.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(ParserTest)
{
    void setup() override
    {

    }

    void teardown() override
    {

    }

    void checkLetStatement(const std::shared_ptr<Statement>& statement, const std::string& name) const
    {
        auto* letStatement = dynamic_cast<LetStatement *>(statement.get());
        CHECK(letStatement->token != nullptr);
        CHECK_EQUAL(Token::LET, (letStatement->token->type));
        CHECK_EQUAL("let", *letStatement->token->literal);
        CHECK(letStatement->identifier != nullptr);
        CHECK_EQUAL(name, *letStatement->identifier->value);
    }
};

TEST(ParserTest, parseSingleLetStatement)
{
    auto lexer = Lexer("let x=5;");
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();

    CHECK_FALSE(program.get()->statements.empty());
    checkLetStatement(program->statements.front(), "x");
}

TEST(ParserTest, parseLetStatements)
{
    auto lexer = Lexer("let x=5;\n"
                       "let y=10;\n"
                       "let foobar = 423432;\n");
    std::vector<std::string> identifiers = {"x", "y", "foobar"};
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();
    CHECK_FALSE(program.get()->statements.empty());
    CHECK(program.get()->statements.size() == 3);

    int i=0;
    for (auto const& statement : program->statements)
    {
        checkLetStatement(statement, identifiers[i]);
        ++i;
    }
}

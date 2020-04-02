/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <vector>
#include "Parser.h"
#include "ast.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(ParserTest)
{
    void setup() override {}
    void teardown() override {}

    void checkLetStatement(const std::shared_ptr<Statement>& statement, const std::string& name) const
    {
        auto* letStatement = dynamic_cast<LetStatement *>(statement.get());
        CHECK(letStatement->token != nullptr);
        CHECK_EQUAL(Token::LET, (letStatement->token->type));
        CHECK_EQUAL("let", *letStatement->token->literal);
        CHECK(letStatement->identifier != nullptr);
        CHECK_EQUAL(name, *letStatement->identifier->value);
    }

    void checkReturnStatement(const std::shared_ptr<Statement>& statement) const
    {
        auto* returnStatement = dynamic_cast<ReturnStatement *>(statement.get());
        CHECK(returnStatement->token != nullptr);
        CHECK_EQUAL(Token::RETURN, (returnStatement->token->type));
        CHECK_EQUAL("return", *returnStatement->token->literal);
    }
};

TEST(ParserTest, parseEmptyProgram)
{
    auto lexer = Lexer("");
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
}

TEST(ParserTest, parseEmptyStatmentProgram)
{
    auto lexer = Lexer(";");
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
}

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
                       "let foobar = 423432\n");
    std::vector<std::string> identifiers = {"x", "y", "foobar"};
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();
    CHECK_FALSE(program.get()->statements.empty());
    CHECK_EQUAL(3, program.get()->statements.size());

    int i=0;
    for (auto const& statement : program->statements)
    {
        checkLetStatement(statement, identifiers[i]);
        ++i;
    }
}

TEST(ParserTest, letStatementErrorMissingAssignment)
{
    std::string message("Expected ASSIGN token. Got INT token (5)");
    auto lexer = Lexer("let x 5;\n");
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
    CHECK_EQUAL(1, parser.errors.size());
    CHECK_EQUAL(message, parser.errors[0]);
}

TEST(ParserTest, letStatementErrorMissingIdentifier)
{
    std::string message("Expected IDENTIFIER token. Got ASSIGN token (=)");
    auto lexer = Lexer("let =10;\n");
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
    CHECK_EQUAL(1, parser.errors.size());
    CHECK_EQUAL(message, parser.errors[0]);
}

TEST(ParserTest, letStatementErrorNonValidIdentifier)
{
    std::string message("Expected IDENTIFIER token. Got INT token (6777)");
    auto lexer = Lexer("let 6777;\n");
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
    CHECK_EQUAL(1, parser.errors.size());
    CHECK_EQUAL(message, parser.errors[0]);
}

TEST(ParserTest, parseSingleReturnStatement)
{
    auto lexer = Lexer("return 5;");
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();

    CHECK_FALSE(program.get()->statements.empty());
    checkReturnStatement(program->statements.front());
}

TEST(ParserTest, parseReturnStatements)
{
    auto lexer = Lexer("return 5;\n"
                       "return 10;\n"
                       "return 423432\n");
    auto parser = Parser(lexer);
    auto program = parser.parseProgram();
    CHECK_FALSE(program.get()->statements.empty());
    CHECK_EQUAL(3, program.get()->statements.size());

    for (auto const& statement : program->statements)
    {
        checkReturnStatement(statement);
    }
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

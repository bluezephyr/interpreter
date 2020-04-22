/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <vector>
#include "Parser.h"
#include "Ast.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

struct TestTuple
{
    std::string input;
    std::string expectedResult;
};

TEST_GROUP(ParserTest)
{
    void setup() override
    {
        lexer = nullptr;
    }

    void teardown() override
    {
        delete (lexer);
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

    void checkReturnStatement(const std::shared_ptr<Statement>& statement) const
    {
        auto* returnStatement = dynamic_cast<ReturnStatement *>(statement.get());
        CHECK(returnStatement->token != nullptr);
        CHECK_EQUAL(Token::RETURN, (returnStatement->token->type));
        CHECK_EQUAL("return", *returnStatement->token->literal);
    }

    void checkIntegerExpression(const std::shared_ptr<Expression>& expression, int64_t value) const
    {
        CHECK(expression != nullptr);
        auto* integer = dynamic_cast<Integer*>(expression.get());
        CHECK(integer != nullptr);
        CHECK_EQUAL(value, integer->value);
        CHECK_EQUAL(std::to_string(value), integer->string());
    }

    void checkInfixIntegerExpression(const char* input,
                                     int left,
                                     const std::string& expectedOp,
                                     int right,
                                     const std::string& expectedOutput)
    {
        auto parser = createParser(input);
        auto program = parser.parseProgram();
        CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
        CHECK_EQUAL(1, program.get()->statements.size());
        std::shared_ptr<Expression> expression = getAndCheckExpressionStatement(program->statements.front());
        auto* infix = dynamic_cast<InfixExpression*>(expression.get());
        CHECK(infix != nullptr);
        checkIntegerExpression(infix->left, left);
        CHECK_EQUAL(expectedOp, infix->op);
        checkIntegerExpression(infix->right, right);
        CHECK_EQUAL(expectedOutput, infix->string());
    }

    static std::shared_ptr<Expression> getAndCheckExpressionStatement(const std::shared_ptr<Statement>& statement)
    {
        auto* expressionStatement = dynamic_cast<ExpressionStatement*>(statement.get());
        CHECK(expressionStatement != nullptr);
        CHECK(expressionStatement->expression != nullptr);
        return expressionStatement->expression;
    }

    void checkParserOutputOk(const TestTuple& test)
    {
        auto l = Lexer(test.input.c_str());
        auto parser = Parser(l);
        auto program = parser.parseProgram();
        CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
        CHECK_EQUAL(test.expectedResult, program->string());
    }

    Parser createParser(const char* input)
    {
        lexer = new Lexer(input);
        auto parser = Parser(*lexer);
        return parser;
    }

    static std::shared_ptr<Program> parse(const std::string& input)
    {
        auto lexer = Lexer(input.c_str());
        auto parser = Parser(lexer);
        auto program = parser.parseProgram();
        return program;
    }

    Lexer *lexer;
};

TEST(ParserTest, parseEmptyProgram)
{
    auto parser = createParser("");
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
}

TEST(ParserTest, parseEmptyStatmentProgramNOK)
{
    // Need to decide whether this is a valid program or not. For now, consider it to be not ok.
    auto parser = createParser(";");
    auto program = parser.parseProgram();
    CHECK(program.get()->statements.empty());
    CHECK_EQUAL(0, program.get()->statements.size());
}

TEST(ParserTest, parseSingleLetStatement)
{
    auto parser = createParser("let x=5;");
    auto program = parser.parseProgram();

    CHECK_FALSE(program.get()->statements.empty());
    checkLetStatement(program->statements.front(), "x");
}

TEST(ParserTest, parseLetStatements)
{
    std::vector<std::string> identifiers = {"x", "y", "foobar"};
    auto parser = createParser("let x=5;\n"
                       "let y=10;\n"
                       "let foobar = 423432\n");
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
    auto parser = createParser("let x 5;\n");
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
    CHECK_EQUAL(1, parser.errors.size());
    CHECK_EQUAL(message, parser.errors[0]);
}

TEST(ParserTest, letStatementErrorMissingIdentifier)
{
    std::string message("Expected IDENTIFIER token. Got ASSIGN token (=)");
    auto parser = createParser("let =10;\n");
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
    CHECK_EQUAL(1, parser.errors.size());
    CHECK_EQUAL(message, parser.errors[0]);
}

TEST(ParserTest, letStatementErrorNonValidIdentifier)
{
    std::string message("Expected IDENTIFIER token. Got INT token (6777)");
    auto parser = createParser("let 6777;\n");
    auto program = parser.parseProgram();
    CHECK_TRUE(program.get()->statements.empty());
    CHECK_EQUAL(1, parser.errors.size());
    CHECK_EQUAL(message, parser.errors[0]);
}

TEST(ParserTest, parseSingleReturnStatement)
{
    auto program = parse("return 5;");
    CHECK_FALSE(program.get()->statements.empty());
    checkReturnStatement(program->statements.front());
}

TEST(ParserTest, parseReturnStatements)
{
    auto parser = createParser("return 5;\n"
                               "return 10;\n"
                               "return 423432\n");
    auto program = parser.parseProgram();
    CHECK_FALSE(program.get()->statements.empty());
    CHECK_EQUAL(3, program.get()->statements.size());

    for (auto const& statement : program->statements)
    {
        checkReturnStatement(statement);
    }
}

TEST(ParserTest, parseIdentifierExpression)
{
    auto parser = createParser("foobar;");
    auto program = parser.parseProgram();
    CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
    CHECK_EQUAL(1, program.get()->statements.size());
    std::shared_ptr<Expression> expression = getAndCheckExpressionStatement(program->statements.front());
    auto* identifier = dynamic_cast<Identifier*>(expression.get());
    CHECK(identifier != nullptr);
    CHECK_EQUAL(Token::IDENTIFIER, (identifier->token->type));
    CHECK_EQUAL("foobar", *identifier->value);
    CHECK_EQUAL("foobar", identifier->string());
}

TEST(ParserTest, parseIntegerLiteralExpression)
{
    auto parser = createParser("5;");
    auto program = parser.parseProgram();
    CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
    CHECK_EQUAL(1, program.get()->statements.size());
    std::shared_ptr<Expression> expression = getAndCheckExpressionStatement(program->statements.front());
    checkIntegerExpression(expression, 5);
}

TEST(ParserTest, parseBangPrefixExpression)
{
    auto parser = createParser("!5;");
    auto program = parser.parseProgram();
    CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
    CHECK_EQUAL(1, program.get()->statements.size());
    std::shared_ptr<Expression> expression = getAndCheckExpressionStatement(program->statements.front());
    auto* prefix = dynamic_cast<PrefixExpression*>(expression.get());
    CHECK(prefix != nullptr);
    CHECK_EQUAL("!", prefix->op);
    checkIntegerExpression(prefix->right, 5);
    CHECK_EQUAL("(!5)", prefix->string());
}

TEST(ParserTest, parseMinusPrefixExpression)
{
    auto parser = createParser("-15");
    auto program = parser.parseProgram();
    CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
    CHECK_EQUAL(1, program.get()->statements.size());
    std::shared_ptr<Expression> expression = getAndCheckExpressionStatement(program->statements.front());
    auto* prefix = dynamic_cast<PrefixExpression*>(expression.get());
    CHECK(prefix != nullptr);
    CHECK_EQUAL("-", prefix->op);
    checkIntegerExpression(prefix->right, 15);
    CHECK_EQUAL("(-15)", prefix->string());
}

TEST(ParserTest, parsePlusInfixExpression)
{
    checkInfixIntegerExpression("5+8;", 5, "+", 8, "(5 + 8)");
}

TEST(ParserTest, parseMinusInfixExpression)
{
    checkInfixIntegerExpression("8-5;", 8, "-", 5, "(8 - 5)");
}

TEST(ParserTest, parseProductInfixExpression)
{
    checkInfixIntegerExpression("3*7;", 3, "*", 7, "(3 * 7)");
}

TEST(ParserTest, parseDivisionInfixExpression)
{
    checkInfixIntegerExpression("24 / 4;", 24, "/", 4, "(24 / 4)");
}

TEST(ParserTest, parseGreaterThanInfixExpression)
{
    checkInfixIntegerExpression("5>4;", 5, ">", 4, "(5 > 4)");
}

TEST(ParserTest, parseLessThanInfixExpression)
{
    checkInfixIntegerExpression("15 < 4;", 15, "<", 4, "(15 < 4)");
}

TEST(ParserTest, parseEqualInfixExpression)
{
    checkInfixIntegerExpression("9 == 4;", 9, "==", 4, "(9 == 4)");
}

TEST(ParserTest, parseNotEqualInfixExpression)
{
    checkInfixIntegerExpression("9 != 4;", 9, "!=", 4, "(9 != 4)");
}

TEST(ParserTest, checkOperatorPrecedenceParsing)
{
    std::vector<TestTuple> tests
    {
        {"-a * b", "((-a) * b)\n"},
        {"!-a", "(!(-a))\n"},
        {"a + b + c", "((a + b) + c)\n"},
        {"a + b - c", "((a + b) - c)\n"},
        {"a * b * c", "((a * b) * c)\n"},
        {"a * b / c", "((a * b) / c)\n"},
        {"a + b / c", "(a + (b / c))\n"},
        {"a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)\n"},
        {"3 + 4; -5 * 5", "(3 + 4)\n((-5) * 5)\n"},
        {"5 > 4 == 3 < 4", "((5 > 4) == (3 < 4))\n"},
        {"5 < 4 != 3 > 4", "((5 < 4) != (3 > 4))\n"},
        {"3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))\n"},
    };

    for (const auto& test: tests)
    {
        checkParserOutputOk(test);
    }
}

TEST(ParserTest, checkGroupedPrecedenceParsing)
{
    std::vector<TestTuple> tests
    {
        {"(3 + 4);", "(3 + 4)\n"},
        {"(3 + 4)", "(3 + 4)\n"},
        {"(3 + 4)(-5 * 5)", "(3 + 4)\n((-5) * 5)\n"},
    };

    for (const auto& test: tests)
    {
        checkParserOutputOk(test);
    }
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}


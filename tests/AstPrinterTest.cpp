/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Lexer.h"
#include "Parser.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "AstPrinter.h"

TEST_GROUP(PrinterTest)
{
    void setup() override {}
    void teardown() override {}

    static std::shared_ptr<Node> parseProgram(const char* input)
    {
        auto l = Lexer(input);
        auto parser = Parser(l);
        auto program = parser.parseProgram();
        CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
        return program;
    }

    static std::string printProgram(const char* input)
    {
        auto printer = AstPrinter();
        return printer.printCode(parseProgram(input));
    }
};

TEST(PrinterTest, printIntegerLiteralStatementCode)
{
    CHECK_EQUAL(std::string("5;"), printProgram("5;"));
}

TEST(PrinterTest, printBooleanTrueLiteralStatementCode)
{
    CHECK_EQUAL(std::string("true;"), printProgram("true;"));
}

TEST(PrinterTest, printBooleanFalseLiteralStatementCode)
{
    CHECK_EQUAL(std::string("false;"), printProgram("false;"));
}

TEST(PrinterTest, printIdentifierStatementCode)
{
    CHECK_EQUAL(std::string("x;"), printProgram("x;"));
}

TEST(PrinterTest, printBangPrefixExpressionStatementCode)
{
    CHECK_EQUAL(std::string("!x;"), printProgram("!x"));
}

TEST(PrinterTest, printManyStatementsCode)
{
    CHECK_EQUAL(std::string("x;5;y;z;"), printProgram("x;5;y;z;"));
}

TEST(PrinterTest, printMinusPrefixExpression)
{
    CHECK_EQUAL(std::string("-x;"), printProgram("-x;"));
}

TEST(PrinterTest, printIntegerPlusInfixExpression)
{
    CHECK_EQUAL(std::string("5+8;"), printProgram("5+8;"));
}

TEST(PrinterTest, printIntegerMinusInfixExpression)
{
    CHECK_EQUAL(std::string("5-8;"), printProgram("5-8;"));
}

TEST(PrinterTest, printManyInfixExpressions)
{
    CHECK_EQUAL(std::string("5*8;6/3;2+4;7-9;"), printProgram("5*8;6/3;2+4;7-9;"));
}

TEST(PrinterTest, printComplexInfixExpressions)
{
    CHECK_EQUAL(std::string("5*8+6/3-2+4*7-9;"), printProgram("5*(8+6)/3-2+4*7-9;"));
}

TEST(PrinterTest, printInfixPrefixExpression)
{
    CHECK_EQUAL(std::string("2--2;"), printProgram("2--2"));
}

TEST(PrinterTest, printSimpleLetStatement)
{
    CHECK_EQUAL(std::string("let x=5;"), printProgram("let x=5"));
}

TEST(PrinterTest, printComplexLetStatement)
{
    CHECK_EQUAL(std::string("let x=5+6;"), printProgram("let x=5+6"));
}

TEST(PrinterTest, printReturnStatement)
{
    CHECK_EQUAL(std::string("return 6;"), printProgram("return 6;"));
}

TEST(PrinterTest, printSimpleIfStatement)
{
    CHECK_EQUAL(std::string("if (x<10) {let y=5;x;}"),
                printProgram("if(x<10) {let y=5;x;}"));
}

TEST(PrinterTest, printIfStatementWithElse)
{
    CHECK_EQUAL(std::string("if (x<10) {let y=5;x;} else {z;}"),
                printProgram("if(x<10) {let y=5;x;} else {z}"));
}

TEST(PrinterTest, printFunctionLiteralWithoutParameters)
{
    CHECK_EQUAL(std::string("fn() {return 10;}"), printProgram("fn() {return 10;}"));
}

TEST(PrinterTest, printFunctionLiteralWithOneParameter)
{
    CHECK_EQUAL(std::string("fn(x) {return 10;}"),
                printProgram("fn(x) {return 10;}"));
}

TEST(PrinterTest, printFunctionLiteralWithParameters)
{
    CHECK_EQUAL(std::string("fn(x, y, z) {return 10;}"),
                printProgram("fn(x, y, z) {return 10;}"));
}

TEST(PrinterTest, printFunctionCallWithoutParameters)
{
    CHECK_EQUAL(std::string("foo();"), printProgram("foo();"));
}

TEST(PrinterTest, printFunctionCallWithOneParameter)
{
    CHECK_EQUAL(std::string("foo(5);"), printProgram("foo(5);"));
}

TEST(PrinterTest, printFunctionCallWithParameters)
{
    CHECK_EQUAL(std::string("foo(5, x, 4-y);"),
                printProgram("foo(5, x, 4-y);"));
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}


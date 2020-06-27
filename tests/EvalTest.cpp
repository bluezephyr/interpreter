/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include <Evaluator.h>
#include "Lexer.h"
#include "Parser.h"
#include "Object.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

struct IntegerTestSetup
{
    const char* input;
    int64_t expected;
};

struct BooleanTestSetup
{
    const char* input;
    bool expected;
};

TEST_GROUP(EvalTest)
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

    static std::shared_ptr<Object> evaluateProgram(const char* input)
    {
        auto evaluator = Evaluator();
        return evaluator.eval(parseProgram(input));
    }
};

TEST(EvalTest, evalPositiveIntegerExpression)
{
    auto evaluated = evaluateProgram("5;");
    CHECK_EQUAL(Object::Type::INTEGER, evaluated->getType());
    CHECK_EQUAL(std::string("5"), evaluated->inspect());
    auto* integer = dynamic_cast<IntegerObject*>(evaluated.get());
    CHECK(integer != nullptr);
    CHECK_EQUAL(5, integer->getValue());
}

TEST(EvalTest, evalTrueBooleanExpression)
{
    auto evaluated = evaluateProgram("true;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("true"), evaluated->inspect());
    auto* boolean = dynamic_cast<BooleanObject*>(evaluated.get());
    CHECK(boolean != nullptr);
    CHECK(boolean->getValue());
}

TEST(EvalTest, evalFalseBooleanExpression)
{
    auto evaluated = evaluateProgram("false;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("false"), evaluated->inspect());
    auto* boolean = dynamic_cast<BooleanObject*>(evaluated.get());
    CHECK(boolean != nullptr);
    CHECK_FALSE(boolean->getValue());
}

TEST(EvalTest, bangPrefixOnTrueBooleanReturnsFalse)
{
    auto evaluated = evaluateProgram("!true;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("false"), evaluated->inspect());
}

TEST(EvalTest, bangPrefixOnFalseBooleanReturnsTrue)
{
    auto evaluated = evaluateProgram("!false;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("true"), evaluated->inspect());
}

TEST(EvalTest, bangPrefixOnIntegerReturnsFalse)
{
    auto evaluated = evaluateProgram("!23;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("false"), evaluated->inspect());
}

TEST(EvalTest, bangPrefixOnNullReturnsTrue)
{
    auto evaluated = evaluateProgram("!(3+false);");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("true"), evaluated->inspect());
}

TEST(EvalTest, minusPrefixNegatesInteger)
{
    auto evaluated = evaluateProgram("-5");
    CHECK_EQUAL(Object::Type::INTEGER, evaluated->getType());
    CHECK_EQUAL(std::string("-5"), evaluated->inspect());
}

TEST(EvalTest, evalIntegerExpression)
{
    std::vector<IntegerTestSetup> tests
    {
        {"42;", 42},
        {"-312;", -312},
        {"2+3;", 5},
        {"8-3;", 5},
        {"3-8;", -5},
        {"3*8;", 24},
        {"3*0;", 0},
        {"42/6;", 7},
        {"5+4*2;", 13},
        {"(5+4)*2;", 18},
    };

    for (auto test: tests)
    {
        auto evaluated = evaluateProgram(test.input);
        auto* integer = dynamic_cast<IntegerObject*>(evaluated.get());
        CHECK(integer != nullptr);
        CHECK_EQUAL(test.expected, integer->getValue());
    }
}

TEST(EvalTest, evalBooleanExpression)
{
    std::vector<BooleanTestSetup> tests
    {
        {"true", true},
        {"false", false},
        {"1 < 2", true},
        {"1 > 2", false},
        {"1 < 1", false},
        {"1 > 1", false},
        {"1 == 1", true},
        {"1 != 1", false},
        {"1 == 2", false},
        {"1 != 2", true},
        {"true == true", true},
        {"false == true", false},
        {"true != true", false},
        {"true != false", true},
        {"(1 < 2) == true", true},
        {"(1 > 2) == true", false},
    };

    for (auto test: tests)
    {
        auto evaluated = evaluateProgram(test.input);
        auto* boolean = dynamic_cast<BooleanObject*>(evaluated.get());
        CHECK(boolean != nullptr);
        CHECK_EQUAL(test.expected, boolean->getValue());
    }
}

TEST(EvalTest, arithmeticOperationOnNonIntegerValuesReturnsNull)
{
    std::vector<const char*> tests
    {
       "10+true;"
       "10+12+3+false;"
       "false+false;"
       "false*8+2/true"
       "-false"
       "-true"
    };

    for(auto test: tests)
    {
        auto evaluated = evaluateProgram(test);
        CHECK_EQUAL(Object::Type::NULLOBJECT, evaluated->getType());
    }
}

TEST(EvalTest, evalIfElseExpression)
{
    std::vector<IntegerTestSetup> tests
    {
        {"if (true)  { 10; };", 10},
        {"if (1 < 2) { 10; };", 10},
        {"if (true)  { 10; } else { 20 };", 10},
        {"if (false) { 10; } else { 20 };", 20},
        {"if (1)     { 10; } else { 20 };", 10},
        {"if (1 < 2) { 10; } else { 20 };", 10},
        {"if (1 > 2) { 10; } else { 20 };", 20},
    };

    for (auto test: tests)
    {
        auto evaluated = evaluateProgram(test.input);
        auto* integer = dynamic_cast<IntegerObject*>(evaluated.get());
        CHECK(integer != nullptr);
        CHECK_EQUAL(test.expected, integer->getValue());
    }
}

TEST(EvalTest, failedIfExpressionWithoutElseReturnNull)
{
    std::vector<const char*> tests
    {
        "if (false) {10;}",
        "if (1 > 2) {10;}",
    };

    for (auto test: tests)
    {
        auto evaluated = evaluateProgram(test);
        CHECK_EQUAL(Object::Type::NULLOBJECT, evaluated->getType());
    }
}

TEST(EvalTest, evalReturnStatements)
{
    std::vector<IntegerTestSetup> tests
    {
        {"return 10;", 10},
        {"return 10; 9;", 10},
        {"return 2*5; 9;", 10},
        {"8; return 2*5; 9;", 10},
    };

    for (auto test: tests)
    {
        auto evaluated = evaluateProgram(test.input);
        auto* integer = dynamic_cast<IntegerObject*>(evaluated.get());
        CHECK(integer != nullptr);
        CHECK_EQUAL(test.expected, integer->getValue());
    }
}

TEST(EvalTest, innermostValueReturnedInNestedBlocks)
{
    std::vector<IntegerTestSetup> tests
    {
        {"if (10 > 1) {if (10 > 1) {return 10;} return 1;}", 10},
        {"if (10 > 1) {if (10 > 1) {10;} return 1;}", 1},
        {"if (10 > 1) {if (10 > 1) {if (10 > 1) {return 20;} 10;} return 1;}", 20},
        {"if (10 > 1) {if (1 > 10) {return 10;} return 1;}", 1},
        {"if (10 > 1) {if (10 > 1) {if (1 > 10) {return 20;} return 10;} return 1;}", 10},
    };

    for (auto test: tests)
    {
        auto evaluated = evaluateProgram(test.input);
        auto *integer = dynamic_cast<IntegerObject *>(evaluated.get());
        CHECK(integer != nullptr);
        CHECK_EQUAL(test.expected, integer->getValue());
    }
}


int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}


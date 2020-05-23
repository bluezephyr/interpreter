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

    static std::shared_ptr<Object> parseAndEvaluate(const char* input)
    {
        auto l = Lexer(input);
        auto parser = Parser(l);
        auto program = parser.parseProgram();
        auto evaluator = Evaluator();
        CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
        return evaluator.eval(*program);
    }
};

TEST(EvalTest, evalPositiveIntegerExpression)
{
    auto evaluated = parseAndEvaluate("5;");
    CHECK_EQUAL(Object::Type::INTEGER, evaluated->getType());
    CHECK_EQUAL(std::string("5"), evaluated->inspect());
    auto* integer = dynamic_cast<IntegerObject*>(evaluated.get());
    CHECK(integer != nullptr);
    CHECK_EQUAL(5, integer->getValue());
}

IGNORE_TEST(EvalTest, evalTrueBooleanExpression)
{
    auto evaluated = parseAndEvaluate("true;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("true"), evaluated->inspect());
    auto* boolean = dynamic_cast<BooleanObject*>(evaluated.get());
    CHECK(boolean != nullptr);
    CHECK(boolean->getValue());
}

IGNORE_TEST(EvalTest, evalFalseBooleanExpression)
{
    auto evaluated = parseAndEvaluate("false;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("false"), evaluated->inspect());
    auto* boolean = dynamic_cast<BooleanObject*>(evaluated.get());
    CHECK(boolean != nullptr);
    CHECK_FALSE(boolean->getValue());
}

IGNORE_TEST(EvalTest, bangPrefixOnTrueBooleanReturnsFalse)
{
    auto evaluated = parseAndEvaluate("!true;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("false"), evaluated->inspect());
}

IGNORE_TEST(EvalTest, bangPrefixOnFalseBooleanReturnsTrue)
{
    auto evaluated = parseAndEvaluate("!false;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("true"), evaluated->inspect());
}

IGNORE_TEST(EvalTest, bangPrefixOnIntegerReturnsFalse)
{
    auto evaluated = parseAndEvaluate("!23;");
    CHECK_EQUAL(Object::Type::BOOLEAN, evaluated->getType());
    CHECK_EQUAL(std::string("false"), evaluated->inspect());
}

IGNORE_TEST(EvalTest, minusPrefixNegatesInteger)
{
    auto evaluated = parseAndEvaluate("-5");
    CHECK_EQUAL(Object::Type::INTEGER, evaluated->getType());
    CHECK_EQUAL(std::string("-5"), evaluated->inspect());
}

IGNORE_TEST(EvalTest, evalIntegerExpression)
{
    std::vector<IntegerTestSetup> tests
    {
        {"42;", 42},
        {"-312;", -312},
        {"2+2;", 4},
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
        auto evaluated = parseAndEvaluate(test.input);
        auto* integer = dynamic_cast<IntegerObject*>(evaluated.get());
        CHECK(integer != nullptr);
        CHECK_EQUAL(test.expected, integer->getValue());
    }
}

IGNORE_TEST(EvalTest, evalBooleanExpression)
{
    std::vector<BooleanTestSetup> tests
    {
        {"true", true},
    };

    for (auto test: tests)
    {
        auto evaluated = parseAndEvaluate(test.input);
        auto* boolean = dynamic_cast<BooleanObject*>(evaluated.get());
        CHECK(boolean != nullptr);
        CHECK_EQUAL(test.expected, boolean->getValue());
    }
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}


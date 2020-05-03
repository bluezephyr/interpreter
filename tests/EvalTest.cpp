/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Lexer.h"
#include "Parser.h"
#include "Object.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(EvalTest)
{
    void setup() override {}
    void teardown() override {}

    static std::shared_ptr<Object> parseAndEvaluate(const char* input)
    {
        auto l = Lexer(input);
        auto parser = Parser(l);
        auto program = parser.parseProgram();
        CHECK_EQUAL_TEXT(0, parser.errors.size(), parser.errors[0].c_str());
        return program->eval();
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

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

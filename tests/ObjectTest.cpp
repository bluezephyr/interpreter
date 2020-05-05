/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Object.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(ObjectTest)
{
    void setup() override {}
    void teardown() override {}
};

TEST(ObjectTest, testPositiveIntegerObject)
{
    IntegerObject integer(5);
    CHECK_EQUAL(Object::Type::INTEGER, integer.getType());
    CHECK_EQUAL(5, integer.getValue());
    CHECK_EQUAL(std::string("5"), integer.inspect());
}

TEST(ObjectTest, testNegativeIntegerObject)
{
    IntegerObject integer(-23435);
    CHECK_EQUAL(Object::Type::INTEGER, integer.getType());
    CHECK_EQUAL(-23435, integer.getValue());
    CHECK_EQUAL(std::string("-23435"), integer.inspect());
}

TEST(ObjectTest, integerFromFloatIsTruncated)
{
    IntegerObject integer(56.5);
    CHECK_EQUAL(Object::Type::INTEGER, integer.getType());
    CHECK_EQUAL(56, integer.getValue());
    CHECK_EQUAL(std::string("56"), integer.inspect());
}

TEST(ObjectTest, testTrueBooleanObject)
{
    BooleanObject trueBoolean(true);
    CHECK_EQUAL(Object::Type::BOOLEAN, trueBoolean.getType());
    CHECK_EQUAL(true, trueBoolean.getValue());
    CHECK_EQUAL(std::string("true"), trueBoolean.inspect());
}

TEST(ObjectTest, testFalseBooleanObject)
{
    BooleanObject falseBoolean(false);
    CHECK_EQUAL(Object::Type::BOOLEAN, falseBoolean.getType());
    CHECK_EQUAL(false, falseBoolean.getValue());
    CHECK_EQUAL(std::string("false"), falseBoolean.inspect());
}

TEST(ObjectTest, testNullObject)
{
    NullObject nullObject;
    CHECK_EQUAL(Object::Type::NULLOBJECT, nullObject.getType());
    CHECK_EQUAL(std::string("null"), nullObject.inspect());
}

TEST(ObjectTest, bangPrefixBooleanTrueReturnsFalse)
{
    BooleanObject boolean(true);
    CHECK_EQUAL(std::string("false"), boolean.evalBangPrefixExpression()->inspect());
}

TEST(ObjectTest, bangPrefixBooleanFalseReturnsTrue)
{
    BooleanObject boolean(false);
    CHECK_EQUAL(std::string("true"), boolean.evalBangPrefixExpression()->inspect());
}

TEST(ObjectTest, bangPrefixIntegerReturnsFalse)
{
    IntegerObject integer(33);
    CHECK_EQUAL(std::string("false"), integer.evalBangPrefixExpression()->inspect());
}

TEST(ObjectTest, bangPrefixNullReturnsTrue)
{
    NullObject nullObject;
    CHECK_EQUAL(std::string("true"), nullObject.evalBangPrefixExpression()->inspect());
}

TEST(ObjectTest, minusPrefixIntegerReturnsNegativeValue)
{
    IntegerObject integer(25);
    CHECK_EQUAL(std::string("-25"), integer.evalMinusPrefixExpression()->inspect());
}

TEST(ObjectTest, minusPrefixBooleanTrueReturnsNull)
{
    BooleanObject boolean(true);
    CHECK_EQUAL(std::string("null"), boolean.evalMinusPrefixExpression()->inspect());
}

TEST(ObjectTest, minusPrefixBooleanFalseReturnsNull)
{
    BooleanObject boolean(false);
    CHECK_EQUAL(std::string("null"), boolean.evalMinusPrefixExpression()->inspect());
}

TEST(ObjectTest, minusPrefixNullReturnsNull)
{
    NullObject nullObject;
    CHECK_EQUAL(std::string("null"), nullObject.evalMinusPrefixExpression()->inspect());
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

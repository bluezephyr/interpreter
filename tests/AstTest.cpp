/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Ast.h"
#include "Token.h"
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(AstTest)
{
    void setup() override {}
    void teardown() override {}

    std::shared_ptr<Expression> createInfixLtExpression(const std::string& left, const std::string& right)
    {
        std::unique_ptr<Token> ltToken = std::make_unique<Token>("<");
        std::unique_ptr<Token> leftInteger = std::make_unique<Token>(Token::INT, left);
        std::unique_ptr<Token> rightInteger = std::make_unique<Token>(Token::INT, right);
        std::shared_ptr<InfixExpression> expression = std::make_shared<InfixExpression>(std::move(ltToken));
        std::shared_ptr<Integer> leftExp = std::make_shared<Integer>(std::move(leftInteger));
        leftExp->value = std::stoi(left);
        std::shared_ptr<Integer> rightExp = std::make_shared<Integer>(std::move(rightInteger));
        rightExp->value = std::stoi(right);
        expression->left = leftExp;
        expression->right = rightExp;
        expression->op = "<";
        return expression;
    }

    std::shared_ptr<Statement> createIdentifierStatement(const std::string& identifier)
    {
        std::unique_ptr<Token> idToken = std::make_unique<Token>(Token::IDENTIFIER, identifier);
        std::shared_ptr<Identifier> expression = std::make_shared<Identifier>(std::move(idToken));
        expression->value = std::make_shared<std::string>(identifier);
        std::shared_ptr<ExpressionStatement> statement = std::make_shared<ExpressionStatement>();
        statement->expression = expression;
        return statement;
    }
};

TEST(AstTest, testLetStatementString)
{
    std::unique_ptr<Token> identToken = std::make_unique<Token>("x");
    std::shared_ptr<Identifier> identifer = std::make_shared<Identifier>(std::move(identToken));
    std::unique_ptr<Token> letToken = std::make_unique<Token>("let");
    LetStatement statement(std::move(letToken));
    statement.identifier = identifer;
    CHECK_EQUAL("let x = ;", statement.string());
}

TEST(AstTest, testReturnStatementString)
{
    std::unique_ptr<Token> returnToken = std::make_unique<Token>("return");
    ReturnStatement statement(std::move(returnToken));
    CHECK_EQUAL("return ;", statement.string());
}

TEST(AstTest, testIntegerString)
{
    std::unique_ptr<Token> token = std::make_unique<Token>("15");
    Integer integer(std::move(token));
    integer.value = 15;
    CHECK_EQUAL("15", integer.string());
}

TEST(AstTest, testBooleanTrueString)
{
    std::unique_ptr<Token> token = std::make_unique<Token>("true");
    Boolean boolean(std::move(token), true);
    CHECK_EQUAL("true", boolean.string());
}

TEST(AstTest, testBooleanFalseString)
{
    std::unique_ptr<Token> token = std::make_unique<Token>("false");
    Boolean boolean(std::move(token), false);
    CHECK_EQUAL("false", boolean.string());
}

TEST(AstTest, testIfStatementString)
{
    std::unique_ptr<Token> ifToken = std::make_unique<Token>("if");
    IfExpression expression(std::move(ifToken));
    expression.condition = createInfixLtExpression("5", "10");
    expression.consequence = createIdentifierStatement("x");
    expression.alternative = createIdentifierStatement("y");
    CHECK_EQUAL("if (5 < 10) { x } else { y }\n", expression.string());
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

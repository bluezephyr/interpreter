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

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

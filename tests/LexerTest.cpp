/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include <Lexer.h>
#include "CppUTest/TestHarness.h"

TEST_GROUP(Lexer)
{
    Lexer *lexer;

    void setup()
    {
        lexer = new Lexer();
    }

    void teardown()
    {
        delete lexer;
    }
};

TEST(Lexer, lexer_init)
{
}

/*
 * Tests to write:
 * -
 */

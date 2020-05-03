/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include <iostream>
#include "Lexer.h"
#include "Parser.h"

int main()
{
    std::cout << "Monkey Programming Language Interpreter!" << std::endl;
    std::cout << "See https://monkeylang.org/ for more information" << std::endl;
    std::cout << "Type in your commands (exit with CRTL-D)" << std::endl;
    std::cout << ">>> ";
    for (std::string line; std::getline(std::cin, line);)
    {
        if (!line.empty())
        {
            auto l = Lexer(line.c_str());
            auto parser = Parser(l);
            auto program = parser.parseProgram();

            if (!parser.errors.empty())
            {
                for (const auto& error : parser.errors)
                {
                    std::cout << error << std::endl;
                }
            }

            auto evaluated = program->eval();
            if (evaluated != nullptr)
            {
                std::cout << evaluated->inspect() << std::endl;
            }
        }
        std::cout << ">>> ";
    }
    std::cout << std::endl;
    return 0;
}
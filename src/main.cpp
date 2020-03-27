/*
 * Copyright (c) 2019 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include <iostream>
#include "Lexer.h"

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
            auto lexer = Lexer(line.c_str());
            std::unique_ptr<Token> token = lexer.nextToken();
            while (token->type != Token::ENDOFFILE)
            {
                std::cout << *(token->literal) << std::endl;
                token = lexer.nextToken();
            }
        }
        std::cout << ">>> ";
    }
    std::cout << std::endl;
    return 0;
}
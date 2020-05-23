/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"
#include "AstPrinter.h"
#include "Evaluator.h"

class ArgumentParser
{
public:
    ArgumentParser(int argc, char *argv[]) : _runREPL (false), _inputFileName ("")
    {
        if (argc > 1)
        {
            // Parse arguments
            _inputFileName = argv[1];
        }
        else
        {
            _runREPL = true;
        }
    }

    bool runREPL() const
    {
        return _runREPL;
    }

    std::string inputFileName()
    {
        return _inputFileName;
    }

private:
    bool _runREPL;
    std::string _inputFileName;
};

void runREPL()
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
            auto evaluator = Evaluator();


            if (!parser.errors.empty())
            {
                for (const auto &error : parser.errors)
                {
                    std::cout << error << std::endl;
                }
            }

            auto evaluated = evaluator.eval(*program);
            if (evaluated != nullptr)
            {
                std::cout << evaluated->inspect() << std::endl;
            }
        }
        std::cout << ">>> ";
    }
    std::cout << std::endl;
}

void printProgramFromFile(const std::basic_string<char>& filename)
{
    std::fstream fs;
    std::vector<char> input;
    fs.open(filename, std::fstream::in);
    char c;
    while (fs.get(c))
    {
        input.push_back(c);
    }
    fs.close();

    auto l = Lexer(&input[0]);
    auto parser = Parser(l);
    auto program = parser.parseProgram();
    auto printer = AstPrinter();
    std::cout << printer.printCode(program) << std::endl;
}

int main(int argc, char *argv[])
{
    ArgumentParser config(argc, argv);
    if (config.runREPL())
    {
        runREPL();
    }
    else
    {
        printProgramFromFile(config.inputFileName());
    }
    return 0;
}


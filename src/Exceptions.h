/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef INTERPRETER_EXCEPTIONS_H
#define INTERPRETER_EXCEPTIONS_H

#include <exception>

class ParserException : public std::exception {};
class WrongTokenException : public ParserException {};
class InvalidStatementException : public ParserException {};
class EndOfFileException : public ParserException {};


#endif //INTERPRETER_EXCEPTIONS_H

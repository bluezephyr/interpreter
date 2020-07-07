/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#include "ControlToken.h"

ControlToken::ControlToken(std::string string) : _string(std::move(string)) {}

std::string ControlToken::string()
{
    return _string;
}

void ControlToken::accept(AstVisitor &visitor)
{
    visitor.visitControlToken(*this);
}



/*
 * Copyright (c) 2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */
#ifndef INTERPRETER_CONTROLTOKEN_H
#define INTERPRETER_CONTROLTOKEN_H

#include <string>
#include "Object.h"
#include "Ast.h"

class ControlToken : public Node
{
public:
    explicit ControlToken(std::string);
    std::string string() override;
    void accept(AstVisitor &visitor) override;

private:
    std::string _string;
};

#endif //INTERPRETER_CONTROLTOKEN_H

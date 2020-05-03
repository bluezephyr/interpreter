/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Object.h"

IntegerObject::IntegerObject(int64_t value) : value(value) {}

std::string IntegerObject::inspect()
{
    return std::to_string(value);
}

Object::Type IntegerObject::getType()
{
    return Object::INTEGER;
}

int64_t IntegerObject::getValue() const
{
    return value;
}

BooleanObject::BooleanObject(bool value) : value(value) {}

std::string BooleanObject::inspect()
{
    return std::string(value ? "true" : "false");
}

Object::Type BooleanObject::getType()
{
    return Object::BOOLEAN;
}

bool BooleanObject::getValue() const
{
    return value;
}

std::string NullObject::inspect()
{
    return std::string("null");
}

Object::Type NullObject::getType()
{
    return NULLOBJECT;
}


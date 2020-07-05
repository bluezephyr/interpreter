/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#include "Object.h"

std::string ErrorObject::inspect()
{
    return std::string();
}

Object::Type ErrorObject::getType()
{
    return ERROR;
}

std::shared_ptr<Object> ErrorObject::evalMinusPrefixExpression()
{
    return std::make_shared<NullObject>();
}

std::shared_ptr<Object> ErrorObject::evalBangPrefixExpression()
{
    return std::make_shared<NullObject>();
}

std::string NullObject::inspect()
{
    return std::string("null");
}

Object::Type NullObject::getType()
{
    return NULLOBJECT;
}

std::shared_ptr<Object> NullObject::evalBangPrefixExpression()
{
    return std::make_shared<BooleanObject>(true);
}

std::shared_ptr<Object> NullObject::evalMinusPrefixExpression()
{
    return std::make_shared<NullObject>();
}
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

std::shared_ptr<Object> IntegerObject::evalBangPrefixExpression()
{
    return std::make_shared<BooleanObject>(false);
}

std::shared_ptr<Object> IntegerObject::evalMinusPrefixExpression()
{
    return std::make_shared<IntegerObject>(-value);
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

std::shared_ptr<Object> BooleanObject::evalBangPrefixExpression()
{
    return std::make_shared<BooleanObject>(!value);
}

std::shared_ptr<Object> BooleanObject::evalMinusPrefixExpression()
{
    return std::make_shared<NullObject>();
}



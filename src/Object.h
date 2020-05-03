/*
 * Copyright (c) 2019-2020 Blue Zephyr
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef INTERPRETER_OBJECT_H
#define INTERPRETER_OBJECT_H

#include <string>

class Object
{
public:
    enum Type
    {
        INTEGER,
        BOOLEAN
    };

    virtual ~Object() = default;
    virtual std::string inspect() = 0;
    virtual enum Type getType() = 0;
};

class IntegerObject : public Object
{
public:
    explicit IntegerObject(int64_t value);
    ~IntegerObject() override = default;
    std::string inspect() override;
    Type getType() override;
    int64_t getValue() const;

private:
    int64_t value;
};

class BooleanObject : public Object
{
public:
    explicit BooleanObject(bool value);
    ~BooleanObject() override = default;
    std::string inspect() override;
    Type getType() override;
    bool getValue() const;

private:
    bool value;
};

#endif //INTERPRETER_OBJECT_H


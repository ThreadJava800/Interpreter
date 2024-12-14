#pragma once

#include <map>
#include <stack>
#include <string>
#include <utility>
#include <iostream>

enum class DataType
{
    INTEGER,
    STRING,
    NONE
};

enum class ArithmeticOperators
{
    ADD
};

class ASTNodeValue
{
public:
    ASTNodeValue(const int val_) :
        type    (DataType::INTEGER),
        int_val (val_)
    {}

    ASTNodeValue(const std::string val_) :
        type       (DataType::STRING),
        string_val (std::move(val_))
    {
        std::cerr << "STR ctor called\n";
        fflush(stderr);
    }

    ASTNodeValue(const ASTNodeValue& rhs) :
        type (rhs.type)
    {
        std::cerr << "Copy ctor\n";
        switch (type)
        {
        case DataType::INTEGER:
            std::cerr << "ITS INT\n";
            fflush(stderr);
            int_val = rhs.int_val;
            break;
        case DataType::STRING:
            std::cerr << "ITS str\n";
            fflush(stderr);
            string_val = rhs.string_val;    // stupid copying
            break;
        default:
            break;
        }
        std::cerr << "Copy ctor finish\n";
        fflush(stderr);
    }

    ASTNodeValue(ASTNodeValue&& rhs)
    {
        std::cerr << "Move ctor\n";
        switch (type)
        {
        case DataType::INTEGER:
            int_val = std::exchange(rhs.int_val, 0);
            break;
        case DataType::STRING:
            string_val = std::move(rhs.string_val);
            break;
        default:
            break;
        }

        type = std::exchange(rhs.type, DataType::NONE);
        std::cerr << "Move ctor finished\n";
    }
        
    ~ASTNodeValue()
    {
        if (type == DataType::STRING)
        {
            string_val.~basic_string();
        }
    }

    DataType type;
    union
    {
        int int_val;
        std::string string_val;
    };

};

class ASTNode
{
public:
    virtual ASTNodeValue getValue() const = 0;
    virtual ~ASTNode() {};
};

class ValueASTNode : public ASTNode
{
public:
    explicit ValueASTNode(const ASTNodeValue value_) :
        value (value_)
    {}

    ASTNodeValue getValue() const override
    {
        return value;
    }

private:
    ASTNodeValue value;
};

class ArithmeticsASTNode : public ASTNode
{
public:
    explicit ArithmeticsASTNode(const ArithmeticOperators oper_, const ASTNode *left_, const ASTNode *right_) :
        oper  (oper_),
        left  (left_),
        right (right_)
    {}

    ASTNodeValue getValue() const override;

private:
    const ASTNode *left, *right;
    ArithmeticOperators oper;
};

class Command
{
public:
    virtual void execute() = 0;
    virtual ~Command() {};
};

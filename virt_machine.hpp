#pragma once

#include <map>
#include <stack>
#include <string>
#include <string_view>
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
    ADD,
    SUB,
    MUL,
    DIV
};

class ASTNodeValue
{
public:
    explicit ASTNodeValue() :
        type    (DataType::NONE),
        int_val (0)
    {}
    ASTNodeValue(const int val_) :
        type    (DataType::INTEGER),
        int_val (val_)
    {}
    ASTNodeValue(const std::string val_) :
        type       (DataType::STRING),
        string_val (std::move(val_))
    {}

    ASTNodeValue(const ASTNodeValue &rhs);
    ASTNodeValue &operator=(const ASTNodeValue &rhs);
    ASTNodeValue(ASTNodeValue &&rhs);
    ASTNodeValue &operator=(ASTNodeValue &&rhs);
        
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

class VariableASTNode : public ASTNode
{
public:
    explicit VariableASTNode(const std::string var_name_) :
        var_name (var_name_)
    {}

    ASTNodeValue getValue() const override;

private:
    std::string var_name;
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

class CreateVarCommand : public Command
{
public:
    explicit CreateVarCommand(const DataType var_type_, const std::string var_name_) :
        var_type (var_type_),
        var_name (var_name_)
    {}

    void execute() override;

private:
    DataType var_type;
    std::string var_name;
};

class PrintCommand : public Command
{
public:
    explicit PrintCommand(const ASTNode *ast_node_) :
        ast_node (ast_node_)
    {}

    void execute() override;

private:
    const ASTNode *ast_node;
};

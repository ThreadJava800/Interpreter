#include <assert.h>
#include <iostream>

#include "virt_machine.hpp"

std::map<std::string, ASTNodeValue> variables;

ASTNodeValue::ASTNodeValue(const ASTNodeValue &rhs) :
        type (rhs.type)
{
    switch (type)
    {
    case DataType::INTEGER:
        int_val = rhs.int_val;
        break;
    case DataType::STRING:
        string_val = rhs.string_val;
        break;
    default:
        break;
    }
}

ASTNodeValue &ASTNodeValue::operator=(const ASTNodeValue &rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    type = rhs.type;
    switch (type)
    {
    case DataType::INTEGER:
        int_val = rhs.int_val;
        break;
    case DataType::STRING:
        string_val = rhs.string_val;
        break;
    default:
        break;
    }

    return *this;
}

ASTNodeValue::ASTNodeValue(ASTNodeValue &&rhs) :
    type (std::exchange(rhs.type, DataType::NONE))
{
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
}

ASTNodeValue &ASTNodeValue::operator=(ASTNodeValue &&rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    type = std::exchange(rhs.type, DataType::NONE);
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

    return *this;
}

ASTNodeValue ArithmeticsASTNode::getValue() const
{
    assert(left);
    assert(right);

    const ASTNodeValue left_val  = std::move(left->getValue());
    const ASTNodeValue right_val = std::move(right->getValue());

    ASTNodeValue result;

    if (left_val.type != DataType::INTEGER || right_val.type != DataType::INTEGER)
    {
        std::cerr << "Attempt to operate incorrect types! Aborting!\n";
        exit(-1);
    }

    switch (oper)
    {
    case ArithmeticOperators::ADD:
        result = left_val.int_val + right_val.int_val;
        break;
    case ArithmeticOperators::SUB:
        result = left_val.int_val - right_val.int_val;
        break;
    case ArithmeticOperators::MUL:
        result = left_val.int_val * right_val.int_val;
        break;
    case ArithmeticOperators::DIV:
        result = left_val.int_val / right_val.int_val;
        break;
    default:
        break;
    }
    
    delete left;
    delete right;

    return result;
}

ASTNodeValue VariableASTNode::getValue() const
{
    return variables[var_name];
}

void CreateVarCommand::execute()
{
    switch (var_type)
    {
    case DataType::INTEGER:
        variables[var_name] = 0;
        break;
    case DataType::STRING:
        variables[var_name] = std::string();
    default:
        break;
    }
}

void PrintCommand::execute()
{
    assert(ast_node);

    const ASTNodeValue node_val = std::move(ast_node->getValue());

    switch (node_val.type)
    {
    case DataType::INTEGER:
        std::cout << node_val.int_val << '\n';
        break;
    case DataType::STRING:
        std::cout << node_val.string_val << '\n';
        break;
    default:
        break;
    }

    delete ast_node;
}

void AssignCommand::execute()
{
    assert(ast_node);

    variables[var_name] = std::move(ast_node->getValue());

    delete ast_node;
}
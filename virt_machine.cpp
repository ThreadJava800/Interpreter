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

    const ASTNodeValue left_val  = left->getValue();
    const ASTNodeValue right_val = right->getValue();

    if (left_val.type != DataType::INTEGER || right_val.type != DataType::INTEGER)
    {
        std::cerr << "Attempt to add incorrect types! Aborting!\n";
        exit(-1);
    }

    switch (oper)
    {
    case ArithmeticOperators::ADD:
        std::cout << left_val.int_val + right_val.int_val << '\n';
        break;
    case ArithmeticOperators::SUB:
        std::cout << left_val.int_val - right_val.int_val << '\n';
        break;
    case ArithmeticOperators::MUL:
        std::cout << left_val.int_val * right_val.int_val << '\n';
        break;
    case ArithmeticOperators::DIV:
        std::cout << left_val.int_val / right_val.int_val << '\n';
        break;
    default:
        break;
    }
    
    delete left;
    delete right;

    return 0;
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
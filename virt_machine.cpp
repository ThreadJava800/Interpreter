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

    int result = 0;

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

    return result;
}

ASTNodeValue ComparatorASTNode::getValue() const
{
    assert(left);
    assert(right);

    const ASTNodeValue left_val  = std::move(left->getValue());
    const ASTNodeValue right_val = std::move(right->getValue());

    int result = 0;

    if (left_val.type != DataType::INTEGER || right_val.type != DataType::INTEGER)
    {
        std::cerr << "Attempt to compare incorrect types! Aborting!\n";
        exit(-1);
    }

    switch (oper)
    {
    case ComparatorOperators::EQ:
        result = left_val.int_val == right_val.int_val;
        break;
    case ComparatorOperators::LESS:
        result = left_val.int_val < right_val.int_val;
        break;
    case ComparatorOperators::LESS_OR_EQ:
        result = left_val.int_val <= right_val.int_val;
        break;
    case ComparatorOperators::MORE:
        result = left_val.int_val > right_val.int_val;
        break;
    case ComparatorOperators::MORE_OR_EQ:
        result = left_val.int_val >= right_val.int_val;
        break;
    default:
        break;
    }

    return result;
}

ASTNodeValue LogicNotASTNode::getValue() const
{
    assert(val);

    const ASTNodeValue node_val  = std::move(val->getValue());
    if (node_val.type != DataType::INTEGER)
    {
        std::cerr << "Attempt to operate logically on incorrect types! Aborting!\n";
        exit(-1);
    }
    
    return !node_val.int_val;
}

ASTNodeValue LogicAndOrASTNode::getValue() const
{
    assert(left);
    assert(right);

    const ASTNodeValue left_val  = std::move(left->getValue());
    const ASTNodeValue right_val = std::move(right->getValue());

    int result = 0;

    switch (oper)
    {
    case LogicOperators::OR:
        result = left_val.int_val || right_val.int_val;
        break;
    case LogicOperators::AND:
        result = left_val.int_val && right_val.int_val;
        break;
    default:
        break;
    }

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
}

void AssignCommand::execute()
{
    assert(ast_node);
    variables[var_name] = std::move(ast_node->getValue());
}

void IfCommand::execute()
{
    assert(bool_part);
    assert(if_com);

    const ASTNodeValue case_val = std::move(bool_part->getValue());

    if (case_val.type != DataType::INTEGER)
    {
        std::cerr << "Attempt to use if case as not numeric value! Aborting!\n";
        exit(-1);
    }

    if (case_val.int_val)
    {
        for (const auto com : *(if_com))
        {
            com->execute();
        }
    }
    else if (else_com)
    {
        for (const auto com : *(else_com))
        {
            com->execute();
        }
    }
}


IfCommand::~IfCommand()
{
    delete bool_part;

    for (const auto com : *(if_com))
    {
        delete com;
    }
    delete if_com;

    if (else_com)
    {
        for (const auto com : *(else_com))
        {
            delete com;
        }
        delete else_com;
    }
}

void WhileCommand::execute()
{
    assert(bool_part);
    assert(cycle_com);

    ASTNodeValue case_val = std::move(bool_part->getValue());
    if (case_val.type != DataType::INTEGER)
    {
        std::cerr << "Attempt to use while case as not numeric value! Aborting!\n";
        exit(-1);
    }

    while (case_val.int_val)
    {
        for (const auto com : *(cycle_com))
        {
            com->execute();
        }

        case_val = std::move(bool_part->getValue());
        if (case_val.type != DataType::INTEGER)
        {
            std::cerr << "Attempt to use while case as not numeric value! Aborting!\n";
            exit(-1);
        }
    }
}

WhileCommand::~WhileCommand()
{
    delete bool_part;

    for (const auto com : *(cycle_com))
    {
        delete com;
    }
    delete cycle_com;
}

void ForCommand::execute()
{
    assert(start_val);
    assert(end_val);
    assert(cycle_com);

    const VariableASTNode cycle_var(start_var_name);
    AssignCommand var_setter(start_var_name, 0);

    var_setter.renewValue(start_val);
    var_setter.execute();

    ASTNodeValue cycle_var_int = std::move(cycle_var.getValue());
    const ASTNodeValue end_val_int = std::move(end_val->getValue());

    if (cycle_var_int.type != DataType::INTEGER || end_val_int.type != DataType::INTEGER)
    {
        std::cerr << "Attempt to use for case as not numeric value! Aborting!\n";
        exit(-1);
    }

    int i;
    if (cycle_step >= 0)
    {
        for (
                i = cycle_var_int.int_val; 
                i < end_val_int.int_val; 
                i += cycle_step, var_setter.renewValue(new ValueASTNode(i)), var_setter.execute()
            )
        {
            for (const auto com : *(cycle_com))
            {
                com->execute();
            }
        }
    }
    else
    {
        for (
                i = cycle_var_int.int_val; 
                i > end_val_int.int_val; 
                i += cycle_step, var_setter.renewValue(new ValueASTNode(i)), var_setter.execute()
            )
        {
            for (const auto com : *(cycle_com))
            {
                com->execute();
            }
        }
    }
}

ForCommand::~ForCommand()
{
    delete end_val;

    for (const auto com : *(cycle_com))
    {
        delete com;
    }
    delete cycle_com;
}

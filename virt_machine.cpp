#include <assert.h>
#include <iostream>

#include "virt_machine.hpp"

std::map<std::string, ASTNodeValue> variables;

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
    default:
        break;
    }
    
    delete left;
    delete right;

    return 0;
}
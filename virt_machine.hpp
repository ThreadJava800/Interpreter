#pragma once

#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

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

enum class ComparatorOperators
{
    LESS,
    LESS_OR_EQ,
    MORE,
    MORE_OR_EQ,
    EQ
};

enum class LogicOperators
{
    AND,
    OR
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
        var_name (std::move(var_name_))
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

    ~ArithmeticsASTNode()
    {
        delete left;
        delete right;
    }

private:
    const ASTNode *left, *right;
    ArithmeticOperators oper;
};

class ComparatorASTNode : public ASTNode
{
public:
    explicit ComparatorASTNode(const ComparatorOperators oper_, const ASTNode *left_, const ASTNode *right_) :
        oper  (oper_),
        left  (left_),
        right (right_)
    {}

    ASTNodeValue getValue() const override;

    ~ComparatorASTNode()
    {
        delete left;
        delete right;
    }

private:
    const ASTNode *left, *right;
    ComparatorOperators oper;
};

class LogicNotASTNode : public ASTNode
{
public:
    explicit LogicNotASTNode(const ASTNode *val_) :
        val (val_)
    {}

    ASTNodeValue getValue() const override;

    ~LogicNotASTNode()
    {
        delete val;
    }

private:
    const ASTNode *val;
};

class LogicAndOrASTNode : public ASTNode
{
public:
    explicit LogicAndOrASTNode(const LogicOperators oper_, const ASTNode *left_, const ASTNode *right_) :
        oper  (oper_),
        left  (left_),
        right (right_)
    {}

    ASTNodeValue getValue() const override;

    ~LogicAndOrASTNode()
    {
        delete left;
        delete right;
    }

private:
    const ASTNode *left, *right;
    LogicOperators oper;
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
        var_name (std::move(var_name_))
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

    ~PrintCommand()
    {
        delete ast_node;
    }

private:
    const ASTNode *ast_node;
};

class AssignCommand : public Command
{
public:
    explicit AssignCommand(const std::string var_name_, const ASTNode *ast_node_) :
        var_name (std::move(var_name_)),
        ast_node (ast_node_)
    {}

    void renewValue(const ASTNode *ast_node_)
    {
        delete ast_node;
        ast_node = ast_node_;
    }

    void execute() override;

    ~AssignCommand()
    {
        delete ast_node;
    }

private:
    const std::string var_name;
    const ASTNode *ast_node;
};

class IfCommand : public Command
{
public:
    explicit IfCommand(
            const ASTNode *bool_part_, 
            const std::vector<Command*> *if_com_, 
            const std::vector<Command*> *else_com_ = nullptr
        ) :
        bool_part (bool_part_),
        if_com    (if_com_),
        else_com  (else_com_)
    {}

    void execute() override;

    ~IfCommand();

private:
    const ASTNode *bool_part;
    const std::vector<Command*> *if_com, *else_com;
};

class WhileCommand : public Command
{
public:
    explicit WhileCommand(
            const ASTNode *bool_part_, 
            const std::vector<Command*> *cycle_com_
        ) :
        bool_part (bool_part_),
        cycle_com (cycle_com_)
    {}

    void execute() override;

    ~WhileCommand();

private:
    const ASTNode *bool_part;
    const std::vector<Command*> *cycle_com;
};

class ForCommand : public Command
{
public:
    explicit ForCommand(
            const std::string start_var_name_,
            const ASTNode *start_val_, 
            const ASTNode *end_val_,
            const std::vector<Command*> *cycle_com_,
            const int cycle_step_
        ) :
        start_var_name (std::move(start_var_name_)),
        start_val      (start_val_),
        end_val        (end_val_),
        cycle_com      (cycle_com_),
        cycle_step     (cycle_step_)
    {}

    void execute() override;

    ~ForCommand();

private:
    const std::string start_var_name;
    const ASTNode *start_val;
    const ASTNode *end_val;
    const std::vector<Command*> *cycle_com;
    const int cycle_step;
};

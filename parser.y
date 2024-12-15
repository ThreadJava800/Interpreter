%language "c++"
%locations
%define api.value.type variant

%code top {
    #include <cassert>
    #include <cstdlib>
    #include <vector>

    #include "virt_machine.hpp"
}

%code provides {
    int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);
}

%token PROGRAM
%token VAR
%token BEGIN_
%token END
%token IF
%token THEN
%token ELSE
%token WHILE
%token WRITELN
%token ASSIGN

%token INTEGER
%token DOUBLE
%token STRING

%token COLON
%token SEMICOLON
%token DOT
%token LEFTBR
%token RIGHTBR

%token <std::string> VAR_NAME
%token <std::string> NUMBER

%precedence NOT

%left ADD
%left SUB
%left MUL
%left DIV
%left EQUALS
%left LESS
%left LESS_OR_EQ
%left MORE
%left MORE_OR_EQ
%left OR
%left AND

%type <ASTNode*> ast_node
%type <ASTNode*> ast_node_and_or
%type <ASTNode*> ast_node_compare
%type <ASTNode*> ast_node_add
%type <ASTNode*> ast_node_mul
%type <ASTNode*> ast_node_brackets
%type <ASTNode*> ast_node_not
%type <ASTNode*> ast_node_leaf
%type <Command*> action
%type <std::vector<Command*>*> actions
%type <Command*> declare_single
%type <std::vector<Command*>*> declare_all

%%

program_entry:
    PROGRAM VAR_NAME SEMICOLON VAR declare_all BEGIN_ actions END DOT
    {
        assert($5);
        assert($7);

        // execute declares
        for (const auto com : *($5))
        {
            com->execute();
            delete com;
        }
        delete $5;

        // execute commands
        for (const auto com : *($7))
        {
            com->execute();
            delete com;
        }
        delete $7;
    }
;

declare_all:
    {
        $$ = new std::vector<Command*>();
    }
|
    declare_all declare_single
    {
        $1->push_back($2);
        $$ = $1;
    }
;

declare_single:
    VAR_NAME COLON INTEGER SEMICOLON
    {
        $$ = new CreateVarCommand(DataType::INTEGER, $1);
    }
|
    VAR_NAME COLON STRING SEMICOLON
    {
        $$ = new CreateVarCommand(DataType::STRING, $1);
    }
;

actions:
    {
        $$ = new std::vector<Command*>();
    }
|
    actions action
    {
        $1->push_back($2);
        $$ = $1;
    }
;

action:
    WRITELN LEFTBR ast_node RIGHTBR
    {
        $$ = new PrintCommand($3);
    }
|
    VAR_NAME ASSIGN ast_node
    {
        $$ = new AssignCommand($1, $3);
    }
|
    IF ast_node THEN BEGIN_ actions END
    {
        $$ = new IfCommand($2, $5);
    }
|
    IF ast_node THEN BEGIN_ actions END ELSE BEGIN_ actions END
    {
        $$ = new IfCommand($2, $5, $9);
    }
;

ast_node:
    ast_node_and_or
    {
        $$ = $1;
    }
;

ast_node_and_or:
    ast_node_compare
    {
        $$ = $1;
    }
|
    ast_node AND ast_node
    {
        $$ = new LogicAndOrASTNode(LogicOperators::AND, $1, $3);
    }
|
    ast_node OR ast_node
    {
        $$ = new LogicAndOrASTNode(LogicOperators::OR, $1, $3);
    }
;

ast_node_compare:
    ast_node_add
    {
        $$ = $1;
    }
|
    ast_node_add LESS ast_node_add
    {
        $$ = new ComparatorASTNode(ComparatorOperators::LESS, $1, $3);
    }
|
    ast_node_add LESS_OR_EQ ast_node_add
    {
        $$ = new ComparatorASTNode(ComparatorOperators::LESS_OR_EQ, $1, $3);
    }
|
    ast_node_add MORE ast_node_add
    {
        $$ = new ComparatorASTNode(ComparatorOperators::MORE, $1, $3);
    }
|
    ast_node_add MORE_OR_EQ ast_node_add
    {
        $$ = new ComparatorASTNode(ComparatorOperators::MORE_OR_EQ, $1, $3);
    }
|
    ast_node_add EQUALS ast_node_add
    {
        $$ = new ComparatorASTNode(ComparatorOperators::EQ, $1, $3);
    }
;

ast_node_add:
    ast_node_mul
    {
        $$ = $1;
    }
|
    ast_node_mul ADD ast_node_mul
    {
        $$ = new ArithmeticsASTNode(ArithmeticOperators::ADD, $1, $3);
    }
|
    ast_node_mul SUB ast_node_mul
    {
        $$ = new ArithmeticsASTNode(ArithmeticOperators::SUB, $1, $3);
    }
;

ast_node_mul:
    ast_node_brackets
    {
        $$ = $1;
    }
|
    ast_node_brackets MUL ast_node_brackets
    {
        $$ = new ArithmeticsASTNode(ArithmeticOperators::MUL, $1, $3);
    }
|
    ast_node_brackets DIV ast_node_brackets
    {
        $$ = new ArithmeticsASTNode(ArithmeticOperators::DIV, $1, $3);
    }
;

ast_node_brackets:
    ast_node_not
    {
        $$ = $1;
    }
|
    LEFTBR ast_node RIGHTBR
    {
        $$ = $2;
    }
;

ast_node_not:
    ast_node_leaf
    {
        $$ = $1;
    }
|
    NOT ast_node
    {
        $$ = new LogicNotASTNode($2);
    }
;

ast_node_leaf:
    NUMBER
    {
        $$ = new ValueASTNode(atoi($1.c_str()));
    }
|
    VAR_NAME
    {
        $$ = new VariableASTNode($1);
    }
;

%%
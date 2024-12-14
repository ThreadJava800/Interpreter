%language "c++"
%locations
%define api.value.type variant

%code top {
    #include <cstdlib>

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

%token INTEGER
%token DOUBLE
%token STRING

%token COLON

%token <std::string> VAR_NAME
%token <std::string> NUMBER

%left ADD
%left SUB
%left MUL
%left DIV

%type <ASTNode*> ast_node

%%

program_entry:
    ast_node
    {
        $1->getValue();
        delete $1;
    }
;

ast_node:
    NUMBER
    {
        $$ = new ValueASTNode(atoi($1.c_str()));
    }
|
    VAR_NAME
    {
        $$ = new VariableASTNode($1);
    }
|
    ast_node ADD ast_node
    {
        $$ = new ArithmeticsASTNode(ArithmeticOperators::ADD, $1, $3);
    }
|
    ast_node SUB ast_node
    {
        $$ = new ArithmeticsASTNode(ArithmeticOperators::SUB, $1, $3);
    }
|
    ast_node MUL ast_node
    {
        $$ = new ArithmeticsASTNode(ArithmeticOperators::MUL, $1, $3);
    }
|
    ast_node DIV ast_node
    {
        $$ = new ArithmeticsASTNode(ArithmeticOperators::DIV, $1, $3);
    }
;

%%
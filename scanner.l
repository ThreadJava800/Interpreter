%option c++ noyywrap yylineno

%{
    #include <iostream>

    #include "virt_machine.hpp"
    #include "parser.hpp"
%}

%%

program                   return yy::parser::token::PROGRAM;
var                       return yy::parser::token::VAR;
begin                     return yy::parser::token::BEGIN_;
end                       return yy::parser::token::END;
if                        return yy::parser::token::IF;
then                      return yy::parser::token::THEN;
else                      return yy::parser::token::ELSE;
while                     return yy::parser::token::WHILE;
writeln                   return yy::parser::token::WRITELN;
do                        return yy::parser::token::DO;

"+"                       return yy::parser::token::ADD;
"-"                       return yy::parser::token::SUB;
"*"                       return yy::parser::token::MUL;
"/"                       return yy::parser::token::DIV;
"="                       return yy::parser::token::EQUALS;
"<"                       return yy::parser::token::LESS;
"<="                      return yy::parser::token::LESS_OR_EQ;
">"                       return yy::parser::token::MORE;
">="                      return yy::parser::token::MORE_OR_EQ;

not                       return yy::parser::token::NOT;
and                       return yy::parser::token::AND;
or                        return yy::parser::token::OR;

":"                       return yy::parser::token::COLON;
";"                       return yy::parser::token::SEMICOLON;
"."                       return yy::parser::token::DOT;
"("                       return yy::parser::token::LEFTBR;
")"                       return yy::parser::token::RIGHTBR;
":="                      return yy::parser::token::ASSIGN;

integer                   return yy::parser::token::INTEGER;
double                    return yy::parser::token::DOUBLE;
string                    return yy::parser::token::STRING;

[a-zA-Z_][a-zA-Z_0-9_]*   return yy::parser::token::VAR_NAME;
[0-9]+                    return yy::parser::token::NUMBER;
[ \t\n]+                  // skip
.                         { std::cerr << "Unexpected character in line(" << lineno() << "): " << YYText() << "'\n"; exit(-1); }

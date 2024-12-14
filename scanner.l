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

"+"                       return yy::parser::token::ADD;

integer                   return yy::parser::token::INTEGER;
double                    return yy::parser::token::DOUBLE;
string                    return yy::parser::token::STRING;

[a-zA-Z_][a-zA-Z_0-9_]*   return yy::parser::token::VAR_NAME;
[0-9]+                    return yy::parser::token::NUMBER;
[ \t\n]+                  // skip
.                         { std::cerr << "Unexpected character in line(" << lineno() << "): " << YYText() << "'\n"; exit(-1); }

#include <assert.h>
#include <FlexLexer.h>
#include <fstream>

#include "virt_machine.hpp"
#include "parser.hpp"

yyFlexLexer *lexer = NULL;

int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc) {
    assert(lexer);

    yylloc->begin.line = lexer->lineno();
    int token = lexer->yylex();
    if(token == yy::parser::token::VAR_NAME || token == yy::parser::token::NUMBER) {
        yylval->build(std::string(lexer->YYText()));
    }

    return token;
}

void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << "Unexpected character in line(" << loc.begin.line << "): " << msg << "'\n";
    exit(-1);
}

int main(int argc, char **argv) {
    if (argc != 2)
    {
        return -1;
    }

    std::ifstream user_input(argv[1]);
    if(!user_input) {
        std::cerr << "Cannot open file: " << argv[1] << '\n';
        return -1;
    }

    lexer = new yyFlexLexer(&user_input);

    yy::parser parser;
    parser.parse();

    delete lexer;
}
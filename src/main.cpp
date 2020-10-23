#include "lexer.h"
#include "parser.h"

int main()
{
    auto tokens = bccc::Tokenize("../examples/return_2.c");
    auto ast = bccc::parseProgram(tokens);

    std::cout << ast;

    return 0;
}
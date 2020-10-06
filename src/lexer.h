#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <vector>
#include <string>

using LineNo = unsigned int;
namespace bccc
{
    std::vector<Token> Tokenize(std::string filename);
    Token getToken(std::string buffer, LineNo line);
}


#endif

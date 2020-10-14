#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <vector>
#include <string>

using LineNo = unsigned int;

namespace bccc
{
    std::vector<Token> Tokenize(const std::string &filename);

    Token getToken(const std::string &buffer, LineNo line);
}

#endif

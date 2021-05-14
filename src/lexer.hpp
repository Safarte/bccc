#ifndef BCCC_LEXER_H
#define BCCC_LEXER_H

#include "token.hpp"

#include <deque>
#include <string>

using LineNo = unsigned int;

namespace bccc {
std::deque<Token> Tokenize(const std::string &filename);

Token getToken(const std::string &buffer, LineNo line);
} // namespace bccc

#endif

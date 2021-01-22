#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

#include <deque>
#include <utility>
#include <vector>

using Tokens = std::deque<bccc::Token>;

namespace bccc
{
    std::pair<AST, Tokens> parseFactor(Tokens &tokens);
    std::pair<AST, Tokens> parseTerm(Tokens &tokens);
    std::pair<AST, Tokens> parseAddExpr(Tokens &tokens);
    std::pair<AST, Tokens> parseRelExpr(Tokens &tokens);
    std::pair<AST, Tokens> parseEqExpr(Tokens &tokens);
    std::pair<AST, Tokens> parseAndExpr(Tokens &tokens);
    std::pair<AST, Tokens> parseExpression(Tokens &tokens);
    std::pair<AST, Tokens> parseStatement(Tokens &tokens);
    std::pair<std::vector<Identifier>, Tokens> parseArguments(Tokens &tokens_);
    std::pair<AST, Tokens> parseFunction(Tokens &tokens);
    AST parseProgram(Tokens &tokens);
} // namespace bccc

#endif
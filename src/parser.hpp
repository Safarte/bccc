#ifndef PARSER_H
#define PARSER_H

#include "ast.hpp"
#include "lexer.hpp"

#include <deque>
#include <utility>
#include <vector>

using Tokens = std::deque<bccc::Token>;

namespace bccc {
AST *parseFactor(Tokens &tokens);
AST *parseTerm(Tokens &tokens);
AST *parseAddExpr(Tokens &tokens);
AST *parseShiftExpr(Tokens &tokens);
AST *parseRelExpr(Tokens &tokens);
AST *parseEqExpr(Tokens &tokens);
AST *parseAndExpr(Tokens &tokens);
AST *parseXorExpr(Tokens &tokens);
AST *parseOrExpr(Tokens &tokens);
AST *parseLAndExpr(Tokens &tokens);
AST *parseLOrExpr(Tokens &tokens);
AST *parseAssignExpr(Tokens &tokens);
AST *parseExpression(Tokens &tokens);
AST *parseStatement(Tokens &tokens);
std::vector<Identifier> parseArguments(Tokens &tokens);
AST *parseFunction(Tokens &tokens);
AST *parseProgram(Tokens &tokens);
} // namespace bccc

#endif
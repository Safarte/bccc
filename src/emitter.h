#ifndef BCCC_EMITTER_H
#define BCCC_EMITTER_H

#include "parser.h"
#include "ast.h"

#include <string>
#include <utility>

namespace bccc
{
    std::pair<std::string, int> emitOperands(AST &op1, AST &op2, int counter = 0);
    std::pair<std::string, int> emitComp(AST &op1, AST &op2, int counter = 0);
    std::pair<std::string, int> emitExpression(AST &expression, int counter = 0);
    std::string emitReturn(AST &ret);
    std::string emitStatement(AST &statement);
    std::string emitFunction(AST& function);
    std::string emitProgram(AST& program);
} // namespace bccc

#endif //BCCC_EMITTER_H

#ifndef BCCC_EMITTER_H
#define BCCC_EMITTER_H

#include "parser.h"
#include "ast.h"

#include <string>

namespace bccc
{
    std::string emitOperands(AST &op1, AST &op2);
    std::string emitExpression(AST &expression);
    std::string emitReturn(AST &ret);
    std::string emitStatement(AST &statement);
    std::string emitFunction(AST &function);
    std::string emitProgram(AST &program);
}

#endif //BCCC_EMITTER_H

#ifndef BCCC_EMITTER_H
#define BCCC_EMITTER_H

#include "ast.hpp"
#include "parser.hpp"

#include <string>
#include <utility>

namespace bccc {
std::pair<std::string, int> emitOperands(AST &op1, AST &op2,
                                         std::map<std::string, int> &varMap,
                                         int counter = 0);
std::pair<std::string, int> emitComp(AST &op1, AST &op2,
                                     std::map<std::string, int> &varMap,
                                     int counter = 0);
std::pair<std::string, int> emitExpression(AST &expression,
                                           std::map<std::string, int> &varMap,
                                           int counter = 0);
std::string emitReturn(AST &ret, std::map<std::string, int> &varMap);
std::string emitStatement(AST &statement, std::map<std::string, int> &varMap,
                          int &stackIndex);
std::string emitFunction(AST &function);
std::string emitProgram(AST &program);
} // namespace bccc

#endif // BCCC_EMITTER_H

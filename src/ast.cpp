#include "ast.hpp"

#include <iostream>
#include <map>
#include <string>

namespace bccc {
std::map<int, std::string> getPostfixOpChar() {
  return std::map<int, std::string>{{0, "++"}, {1, "--"}};
}

std::map<int, std::string> getUnaryOpChar() {
  return std::map<int, std::string>{{0, "+"}, {1, "-"},  {2, "~"},
                                    {3, "!"}, {4, "++"}, {5, "--"}};
}

std::map<int, std::string> getBinaryOpChar() {
  return std::map<int, std::string>{
      {0, "+"},  {1, "-"},  {2, "*"},  {3, "/"},  {4, "&&"},  {5, "||"},
      {6, "=="}, {7, "!="}, {8, "<"},  {9, "<="}, {10, ">"},  {11, ">="},
      {12, "%"}, {13, "&"}, {14, "|"}, {15, "^"}, {16, "<<"}, {17, ">>"}};
}

std::ostream &operator<<(std::ostream &os, AST &ast) {
  auto mUnaryOp = getUnaryOpChar();
  auto mBinaryOp = getBinaryOpChar();

  if (ast.isFuncDef()) {
    auto function = dynamic_cast<FuncDef *>(&ast);
    os << "FUN " << function->name << ":\n";
    os << "\tbody:\n";
    for (auto statement : function->body) {
      os << "\t\t" << *statement << "\n";
    }
  }
  if (ast.isReturn()) {
    auto statement = dynamic_cast<Return *>(&ast);
    os << "RETURN " << *statement->expression;
  }
  if (ast.isBinaryOp()) {
    auto expr = dynamic_cast<BinaryOp *>(&ast);
    os << "(" << *expr->leftOperand << " " << mBinaryOp[(int)expr->op] << " "
       << *expr->rightOperand << ")";
  }
  if (ast.isUnaryOp()) {
    auto expr = dynamic_cast<UnaryOp *>(&ast);
    os << mUnaryOp[(int)expr->op] << *expr->operand;
  }
  if (ast.isInt()) {
    auto lit = dynamic_cast<IntLit *>(&ast);
    os << "Int<" << lit->value << ">";
  }
  if (ast.isDeclare()) {
    auto decl = dynamic_cast<Declare *>(&ast);
    os << "DECLARE Var<" << decl->varName << ">";
    if (decl->expression) {
      os << " = " << *decl->expression.value();
    }
  }
  if (ast.isAssign()) {
    auto assign = dynamic_cast<Assign *>(&ast);
    os << "ASSIGN Var<" << assign->varName << "> = " << *assign->expression;
  }
  if (ast.isVar()) {
    auto var = dynamic_cast<Var *>(&ast);
    os << "Var<" << var->name << ">";
  }

  return os;
}
} // namespace bccc

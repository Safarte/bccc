#include "emitter.hpp"
#include "parser.hpp"

#include <sstream>
#include <string>
#include <utility>

namespace bccc {
std::pair<std::string, int> emitOperands(AST &op1, AST &op2,
                                         std::map<std::string, int> &varMap,
                                         int counter) {
  std::stringstream ss;

  auto [expr1, counter1] = emitExpression(op1, varMap, counter);
  ss << expr1;
  ss << "\tpush\t%rax\n";
  auto [expr2, counter2] = emitExpression(op2, varMap, counter1);
  ss << expr2;
  ss << "\tpop\t%rcx\n";

  return {ss.str(), counter2};
}

std::pair<std::string, int>
emitComp(AST &op1, AST &op2, std::map<std::string, int> &varMap, int counter) {
  std::stringstream ss;

  auto [expr, counter_] = emitOperands(op1, op2, varMap, counter);
  ss << expr;
  ss << "\tcmpl\t%eax, %ecx\n";
  ss << "\tmovl\t$0, %eax\n";

  return {ss.str(), counter_};
}

std::pair<std::string, int> emitExpression(AST &expression,
                                           std::map<std::string, int> &varMap,
                                           int counter) {
  std::stringstream ss;
  int counter_ = counter;

  if (expression.isInt()) {
    auto intKind = dynamic_cast<IntLit *>(&expression);
    ss << "\tmovl\t$" << intKind->value << ", %eax\n";
  }

  if (expression.isVar()) {
    auto var = dynamic_cast<Var *>(&expression);

    if (varMap.count(var->name) <= 0) {
      std::cout << "Variable not declared in this scope";
      exit(0);
    }

    int varOffset = varMap[var->name];
    ss << "\tmov\t" << varOffset << "(%rbp), %rax\n";
  }

  if (expression.isAssign()) {
    auto assignNode = dynamic_cast<Assign *>(&expression);

    ss << emitExpression(*assignNode->expression, varMap).first;
    int varOffset = varMap[assignNode->varName];
    ss << "\tmov\t%rax, " << varOffset << "(%rbp)\n";
  }

  if (expression.isUnaryOp()) {
    auto operation = dynamic_cast<UnaryOp *>(&expression);

    auto [expr, counter1] =
        emitExpression(*operation->operand, varMap, counter);
    counter_ = counter1;
    ss << expr;

    switch (operation->op) {
    case eUnaryOp::Minus:
      ss << "\tneg\t%eax\n";
      break;
    case eUnaryOp::BitwiseNot:
      ss << "\tnot\t%eax\n";
      break;
    case eUnaryOp::LogicalNot:
      ss << "\tcmpl\t$0, %eax\n";
      ss << "\tmovl\t$0, %eax\n";
      ss << "\tsete\t%al\n";
      break;
    }
  }

  if (expression.isBinaryOp()) {
    auto operation = dynamic_cast<BinaryOp *>(&expression);

    switch (operation->op) {
    case eBinaryOp::Add: {
      auto [expr, counter1] = emitOperands(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\taddl\t%ecx, %eax\n";
      break;
    }
    case eBinaryOp::Sub: {
      auto [expr, counter1] = emitOperands(
          *operation->rightOperand, *operation->leftOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tsubl\t%ecx, %eax\n";
      break;
    }
    case eBinaryOp::Mul: {
      auto [expr, counter1] = emitOperands(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\timul\t%ecx, %eax\n";
      break;
    }
    case eBinaryOp::Div: {
      auto [expr, counter1] = emitOperands(
          *operation->rightOperand, *operation->leftOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tcdq\n";
      ss << "\tidiv\t%ecx\n";
      break;
    }
    case eBinaryOp::LAnd: {
      auto [expr1, counter1] =
          emitExpression(*operation->leftOperand, varMap, counter);
      ss << expr1;
      ss << "\tcmpl\t$0, %eax\n";
      ss << "\tjne\t_" << counter1 << "\n";
      ss << "\tmovl\t$1, %eax\n";
      auto [expr2, counter2] =
          emitExpression(*operation->rightOperand, varMap, counter1 + 1);
      ss << "\tjmp\t_" << counter2 << "\n";
      ss << "_" << counter1 << ":\n";
      ss << expr2;
      ss << "\tcmpl\t$0, %eax\n";
      ss << "\tmovl\t$0, %eax\n";
      ss << "\tsetne\t%al\n";
      ss << "_" << counter2 << ":\n";
      counter_ = counter2 + 1;
      break;
    }
    case eBinaryOp::LOr: {
      auto [expr1, counter1] =
          emitExpression(*operation->leftOperand, varMap, counter);
      ss << expr1;
      ss << "\tcmpl\t$0, %eax\n";
      ss << "\tje\t_" << counter1 << "\n";
      ss << "\tmovl\t$1, %eax\n";
      auto [expr2, counter2] =
          emitExpression(*operation->rightOperand, varMap, counter1 + 1);
      ss << "\tjmp\t_" << counter2 << "\n";
      ss << "_" << counter1 << ":\n";
      ss << expr2;
      ss << "\tcmpl\t$0, %eax\n";
      ss << "\tmovl\t$0, %eax\n";
      ss << "\tsetne\t%al\n";
      ss << "_" << counter2 << ":\n";
      counter_ = counter2 + 1;
      break;
    }
    case eBinaryOp::Eq: {
      auto [expr, counter1] = emitComp(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tsete\t%al\n";
      break;
    }
    case eBinaryOp::Ne: {
      auto [expr, counter1] = emitComp(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tsetne\t%al\n";
      break;
    }
    case eBinaryOp::Lt: {
      auto [expr, counter1] = emitComp(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tsetl\t%al\n";
      break;
    }
    case eBinaryOp::Le: {
      auto [expr, counter1] = emitComp(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tsetle\t%al\n";
      break;
    }
    case eBinaryOp::Gt: {
      auto [expr, counter1] = emitComp(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tsetg\t%al\n";
      break;
    }
    case eBinaryOp::Ge: {
      auto [expr, counter1] = emitComp(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tsetge\t%al\n";
      break;
    }
    case eBinaryOp::Mod: {
      auto [expr, counter1] = emitOperands(
          *operation->rightOperand, *operation->leftOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tcdq\n";
      ss << "\tidiv\t%ecx\n";
      ss << "\tmovl\t%edx, %eax\n";
      break;
    }
    case eBinaryOp::And: {
      auto [expr, counter1] = emitOperands(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tand\t%ecx, %eax\n";
      break;
    }
    case eBinaryOp::Or: {
      auto [expr, counter1] = emitOperands(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tor\t%ecx, %eax\n";
      break;
    }
    case eBinaryOp::Xor: {
      auto [expr, counter1] = emitOperands(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\txor\t%ecx, %eax\n";
      break;
    }
    case eBinaryOp::Shr: {
      auto [expr, counter1] = emitOperands(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tshr\t%ecx, %eax\n";
      break;
    }
    case eBinaryOp::Shl: {
      auto [expr, counter1] = emitOperands(
          *operation->leftOperand, *operation->rightOperand, varMap, counter);
      counter_ = counter1;
      ss << expr;
      ss << "\tshl\t%ecx, %eax\n";
      break;
    }
    }
  }

  return {ss.str(), counter_};
}

std::string emitReturn(AST &ret, std::map<std::string, int> &varMap) {
  std::stringstream ss;

  auto retNode = dynamic_cast<Return *>(&ret);

  ss << emitExpression(*retNode->expression, varMap).first;

  ss << "\tmov\t%rbp, %rsp\n";
  ss << "\tpop\t%rbp\n";
  ss << "\tret\n";

  return ss.str();
}

std::string emitDeclare(AST &decl, std::map<std::string, int> &varMap,
                        int &stackIndex) {
  std::stringstream ss;

  auto declNode = dynamic_cast<Declare *>(&decl);

  if (varMap.count(declNode->varName) > 0) {
    std::cout << "Cannot redeclare variable in scope";
    exit(0);
  }

  if (declNode->expression) {
    ss << emitExpression(*declNode->expression.value(), varMap).first;
  } else {
    ss << "\txor\t%eax, %eax\n";
  }
  ss << "\tpush\t%rax\n";
  varMap[declNode->varName] = stackIndex;
  stackIndex -= 8;

  return ss.str();
}

std::string emitStatement(AST &statement, std::map<std::string, int> &varMap,
                          int &stackIndex) {
  std::stringstream ss;

  if (statement.isReturn()) {
    ss << emitReturn(statement, varMap);
  }
  if (statement.isDeclare()) {
    ss << emitDeclare(statement, varMap, stackIndex);
  }
  if (statement.isInt() || statement.isVar() || statement.isUnaryOp() ||
      statement.isBinaryOp() || statement.isAssign()) {
    ss << emitExpression(statement, varMap).first;
  }

  return ss.str();
}

std::string emitFunction(AST &function) {
  std::stringstream ss;
  std::map<std::string, int> varMap;
  int stackIndex = -8;
  int retCount = 0;

  auto func = dynamic_cast<FuncDef *>(&function);

  ss << ".globl " << func->name << "\n";
  ss << func->name << ":\n";
  ss << "\tpush\t%rbp\n";
  ss << "\tmov\t%rsp, %rbp\n";
  for (auto statement : func->body) {
    ss << emitStatement(*statement, varMap, stackIndex);
    if (statement->isReturn()) {
      retCount++;
    }
  }

  if (retCount == 0 && func->name == std::string("main")) {
    ss << "\tmov\t%rbp, %rsp\n";
    ss << "\tpop\t%rbp\n";
    ss << "\txor\t%eax, %eax\n";
    ss << "\tret\n";
  }

  return ss.str();
}

std::string emitProgram(AST &program) {
  std::stringstream ss;

  if (program.isFuncDef()) {
    ss << emitFunction(program);
  }

  return ss.str();
}
} // namespace bccc

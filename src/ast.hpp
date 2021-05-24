#ifndef BCCC_AST_H
#define BCCC_AST_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace bccc {
enum class ePostfixOp { Inc, Dec };

std::map<int, std::string> getPostfixOpChar();

enum class eUnaryOp { Plus, Minus, BitwiseNot, LogicalNot };

std::map<int, std::string> getUnaryOpChar();

enum class eBinaryOp {
  Add,
  Sub,
  Mul,
  Div,
  LAnd,
  LOr,
  Eq,
  Ne,
  Lt,
  Le,
  Gt,
  Ge,
  Mod,
  And,
  Or,
  Xor,
  Shl,
  Shr
};

std::map<int, std::string> getBinaryOpChar();

enum class eASTNodeType {
  FuncDef,
  Int,
  UnaryOp,
  BinaryOp,
  Return,
  Var,
  Assign,
  Declare
};

struct AST {
  std::optional<eASTNodeType> type;

  AST() = default;

  AST(AST &) = default;

protected:
  explicit AST(eASTNodeType type_) : type(type_) {}

public:
  [[nodiscard]] virtual bool isFuncDef() const {
    return type == eASTNodeType::FuncDef;
  }

  [[nodiscard]] virtual bool isInt() const { return type == eASTNodeType::Int; }

  [[nodiscard]] virtual bool isUnaryOp() const {
    return type == eASTNodeType::UnaryOp;
  }

  [[nodiscard]] virtual bool isBinaryOp() const {
    return type == eASTNodeType::BinaryOp;
  }

  [[nodiscard]] virtual bool isReturn() const {
    return type == eASTNodeType::Return;
  }

  [[nodiscard]] virtual bool isVar() const { return type == eASTNodeType::Var; }

  [[nodiscard]] virtual bool isAssign() const {
    return type == eASTNodeType::Assign;
  }

  [[nodiscard]] virtual bool isDeclare() const {
    return type == eASTNodeType::Declare;
  }
};

struct FuncDef : AST {
  std::string name;
  std::vector<AST *> body;

  FuncDef(std::string &name_, std::vector<AST *> body_)
      : name(name_), body(std::move(body_)), AST(eASTNodeType::FuncDef) {}
};

struct IntLit : AST {
  int value;

  explicit IntLit(int value_) : value(value_), AST(eASTNodeType::Int) {}
};

struct UnaryOp : AST {
  eUnaryOp op;
  AST *operand;

  UnaryOp(eUnaryOp op_, AST *operand_)
      : op(op_), operand(operand_), AST(eASTNodeType::UnaryOp) {}
};

struct BinaryOp : AST {
  eBinaryOp op;
  AST *leftOperand;
  AST *rightOperand;

  BinaryOp(eBinaryOp op_, AST *left_, AST *right_)
      : op(op_), leftOperand(left_), rightOperand(right_),
        AST(eASTNodeType::BinaryOp) {}
};

struct Return : AST {
  AST *expression;

  explicit Return(AST *expression_)
      : expression(expression_), AST(eASTNodeType::Return) {}
};

struct Var : AST {
  std::string name;

  explicit Var(std::string &name_) : name(name_), AST(eASTNodeType::Var) {}
};

struct Assign : AST {
  std::string varName;
  AST *expression;

  Assign(std::string &varName_, AST *expression_)
      : varName(varName_), expression(expression_), AST(eASTNodeType::Assign) {}
};

struct Declare : AST {
  std::string varName;
  std::optional<AST *> expression;

  explicit Declare(std::string &name_)
      : varName(name_), AST(eASTNodeType::Declare) {}

  Declare(std::string &name_, AST *expression_)
      : varName(name_), expression(expression_), AST(eASTNodeType::Declare) {}
};

std::ostream &operator<<(std::ostream &os, AST &ast);
} // namespace bccc

#endif // BCCC_AST_H

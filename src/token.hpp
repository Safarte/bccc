#ifndef BCCC_TOKEN_H
#define BCCC_TOKEN_H

#include "ast.hpp"
#include <map>
#include <string>
#include <variant>

namespace bccc {
enum class eKeyword { Int, Return };

std::map<int, std::string> getKeywordStr();

enum class eSymbol {
  OpeningParen,
  ClosingParen,
  OpeningBrace,
  ClosingBrace,
  Semicolon,
  LogicalNot,
  BitwiseNot,
  Add,
  Sub,
  Asterisk,
  Div,
  LogicalAnd,
  LogicalOr,
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
  Shr,
  Assign,
  AssignAdd,
  AssignSub,
  AssignMul,
  AssignDiv,
  AssignMod,
  AssignShl,
  AssignShr,
  AssignAnd,
  AssignOr,
  AssignXor,
  Comma,
  Inc,
  Dec
};

std::map<int, std::string> getSymbolStr();

struct Keyword {
  eKeyword keyword;
};
struct Identifier {
  std::string identifier;
};
struct Int {
  int n;
};
struct Symbol {
  eSymbol symbol;
};
using TokenKind = std::variant<Keyword, Identifier, Int, Symbol>;

struct Token {
  TokenKind kind;

  Token() = delete;

  explicit Token(TokenKind kind_) : kind(std::move(kind_)) {}

  bool isSymbol() const { return kind.index() == 3; }

  bool isSymbol(eSymbol symbol_) const {
    if (kind.index() == 3) {
      auto symbol = std::get<Symbol>(kind);
      return symbol.symbol == symbol_;
    }
    return false;
  }

  bool isUnaryOp() const {
    return isSymbol(eSymbol::Add) || isSymbol(eSymbol::Sub) ||
           isSymbol(eSymbol::BitwiseNot) || isSymbol(eSymbol::LogicalNot);
  }

  bool isPrefixOp() const {
    return isSymbol(eSymbol::Inc) || isSymbol(eSymbol::Dec);
  }

  bool isInt() const { return kind.index() == 2; }

  bool isIdentifier() const { return kind.index() == 1; }

  bool isKeyword() const { return kind.index() == 0; }

  bool isKeyword(eKeyword keyword_) const {
    if (kind.index() == 0) {
      auto keyword = std::get<Keyword>(kind);
      return keyword.keyword == keyword_;
    }
    return false;
  }
};

std::ostream &operator<<(std::ostream &os, Token &token);
} // namespace bccc

#endif // BCCC_TOKEN_H

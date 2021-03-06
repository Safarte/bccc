#include "lexer.hpp"
#include "token.hpp"

#include <deque>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_set>

namespace bccc {
Token getToken(const std::string &buffer, LineNo line) {
  if (buffer == "int") {
    return Token(Keyword{eKeyword::Int});
  }
  if (buffer == "return") {
    return Token(Keyword{eKeyword::Return});
  }
  if (buffer == "(") {
    return Token(Symbol{eSymbol::OpeningParen});
  }
  if (buffer == ")") {
    return Token(Symbol{eSymbol::ClosingParen});
  }
  if (buffer == "{") {
    return Token(Symbol{eSymbol::OpeningBrace});
  }
  if (buffer == "}") {
    return Token(Symbol{eSymbol::ClosingBrace});
  }
  if (buffer == ";") {
    return Token(Symbol{eSymbol::Semicolon});
  }
  if (buffer == "+") {
    return Token(Symbol{eSymbol::Add});
  }
  if (buffer == "-") {
    return Token(Symbol{eSymbol::Sub});
  }
  if (buffer == "*") {
    return Token(Symbol{eSymbol::Asterisk});
  }
  if (buffer == "/") {
    return Token(Symbol{eSymbol::Div});
  }
  if (buffer == "~") {
    return Token(Symbol{eSymbol::BitwiseNot});
  }
  if (buffer == "!") {
    return Token(Symbol{eSymbol::LogicalNot});
  }
  if (buffer == "&&") {
    return Token(Symbol{eSymbol::LogicalAnd});
  }
  if (buffer == "||") {
    return Token(Symbol{eSymbol::LogicalOr});
  }
  if (buffer == "==") {
    return Token(Symbol{eSymbol::Eq});
  }
  if (buffer == "!=") {
    return Token(Symbol{eSymbol::Ne});
  }
  if (buffer == "<") {
    return Token(Symbol{eSymbol::Lt});
  }
  if (buffer == "<=") {
    return Token(Symbol{eSymbol::Le});
  }
  if (buffer == ">") {
    return Token(Symbol{eSymbol::Gt});
  }
  if (buffer == ">=") {
    return Token(Symbol{eSymbol::Ge});
  }
  if (buffer == "%") {
    return Token(Symbol{eSymbol::Mod});
  }
  if (buffer == "&") {
    return Token(Symbol{eSymbol::And});
  }
  if (buffer == "|") {
    return Token(Symbol{eSymbol::Or});
  }
  if (buffer == "^") {
    return Token(Symbol{eSymbol::Xor});
  }
  if (buffer == "<<") {
    return Token(Symbol{eSymbol::Shl});
  }
  if (buffer == ">>") {
    return Token(Symbol{eSymbol::Shr});
  }
  if (buffer == "=") {
    return Token(Symbol{eSymbol::Assign});
  }
  if (buffer == "+=") {
    return Token(Symbol{eSymbol::AssignAdd});
  }
  if (buffer == "-=") {
    return Token(Symbol{eSymbol::AssignSub});
  }
  if (buffer == "*=") {
    return Token(Symbol{eSymbol::AssignMul});
  }
  if (buffer == "/=") {
    return Token(Symbol{eSymbol::AssignDiv});
  }
  if (buffer == "%=") {
    return Token(Symbol{eSymbol::AssignMod});
  }
  if (buffer == "<<=") {
    return Token(Symbol{eSymbol::AssignShl});
  }
  if (buffer == ">>=") {
    return Token(Symbol{eSymbol::AssignShr});
  }
  if (buffer == "&=") {
    return Token(Symbol{eSymbol::AssignAnd});
  }
  if (buffer == "|=") {
    return Token(Symbol{eSymbol::AssignOr});
  }
  if (buffer == "^=") {
    return Token(Symbol{eSymbol::AssignXor});
  }
  if (buffer == ",") {
    return Token(Symbol{eSymbol::Comma});
  }
  if (buffer == "++") {
    return Token(Symbol{eSymbol::Inc});
  }
  if (buffer == "--") {
    return Token(Symbol{eSymbol::Dec});
  }
  if (std::regex_match(buffer, std::regex("[a-zA-Z][a-zA-Z0-9_]*"))) {
    return Token(Identifier{buffer});
  }
  if (std::regex_match(buffer,
                       std::regex("0|[1-9][0-9]*|0x[0-9a-f]+|0[0-7]+"))) {
    return Token(Int{std::stoi(buffer, nullptr, 0)});
  }

  /* Will only reach here if all else fails, invalid string */

  std::cout << "Invalid expression at line number " << line << " : " << buffer;
  exit(0);
}

std::deque<Token> Tokenize(const std::string &filename) {
  std::ifstream file(filename, std::ios::in);

  if (file.fail()) {
    std::cout << "Unable to open file!\n";
    std::cin.get();
    exit(0);
  }

  LineNo line = 1;
  std::string buffer;

  char ch_;

  std::deque<Token> tokens;
  const std::unordered_set<std::string> delims = {"{", "}", "(", ")", ";"};
  const std::unordered_set<std::string> whitespace = {"\n", "\r", "\t", " "};
  const std::unordered_set<std::string> ops = {
      "+",  "-",  "*",   "/",   "&",  "|",  "^",  "%",  "~",  "=",  "!",
      "<<", ">>", "<=",  ">=",  "==", "!=", "&&", "||", "+=", "-=", "*=",
      "/=", "%=", "<<=", ">>=", "&=", "|=", "^=", ",",  "++", "--"};

  while (file >> std::noskipws >> ch_) {
    const std::string ch(1, ch_);
    const std::string nextBuffer = buffer + ch;

    if (ch == "\n" || ch == "\r")
      line += 1;

    if (delims.find(ch) != delims.end()) {
      if (!buffer.empty()) {
        tokens.push_back(getToken(buffer, line));
        buffer = "";
      }
      tokens.push_back(getToken(ch, line));
      continue;
    } else if (whitespace.find(ch) != whitespace.end()) {
      if (!buffer.empty()) {
        tokens.push_back(getToken(buffer, line));
        buffer = "";
      }
      continue;
    } else if (ops.find(nextBuffer) != ops.end()) {
      buffer = nextBuffer;
    } else if (ops.find(buffer) != ops.end() || ops.find(ch) != ops.end()) {
      tokens.push_back(getToken(buffer, line));
      buffer = ch;
    } else {
      buffer += ch;
    }
  }

  return tokens;
}
} // namespace bccc
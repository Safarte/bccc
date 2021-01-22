#include "ast.h"

#include <iostream>
#include <map>

namespace bccc
{
    std::map<int, std::string> getKeywordStr()
    {
        return std::map<int, std::string>{
            {0, "kwd:int"},
            {1, "kwd:return"}};
    }

    std::map<int, std::string> getSymbolStr()
    {
        return std::map<int, std::string>{
            {0, "("},
            {1, ")"},
            {2, "{"},
            {3, "}"},
            {4, ";"},
            {5, "!"},
            {6, "~"},
            {7, "+"},
            {8, "-"},
            {9, "*"},
            {10, "/"},
            {11, "&&"},
            {12, "||"},
            {13, "=="},
            {14, "!="},
            {15, "<"},
            {16, "<="},
            {17, ">"},
            {18, ">="},
            {19, "%"},
            {20, "&"},
            {21, "|"},
            {22, "^"},
            {23, "<<"},
            {24, ">>"}};
    }

    std::ostream &operator<<(std::ostream &os, Token &token)
    {
        auto mKeyword = getKeywordStr();
        auto mSymbol = getSymbolStr();

        switch (token.kind.index())
        {
        case 0: // Keyword
        {
            auto [keyword] = std::get<Keyword>(std::move(token.kind));
            os << mKeyword[(int)keyword];
            break;
        }
        case 1: // Identifier
        {
            auto [identifier] = std::get<Identifier>(std::move(token.kind));
            os << "id:" << identifier;
            break;
        }
        case 2: // Int
        {
            auto [n] = std::get<Int>(std::move(token.kind));
            os << "int:" << n;
            break;
        }
        case 3: // Symbol
        {
            auto [symbol] = std::get<Symbol>(std::move(token.kind));
            os << mSymbol[(int)symbol];
            break;
        }
        default:
            break;
        }
        return os;
    }
} // namespace bccc

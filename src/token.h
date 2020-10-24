#ifndef BCCC_TOKEN_H
#define BCCC_TOKEN_H

#include <string>
#include <variant>

namespace bccc
{
    enum class eKeyword
    {
        Int,
        Return
    };

    enum class eSymbol
    {
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
        Div
    };

    struct Keyword
    {
        eKeyword keyword;
    };
    struct Identifier
    {
        std::string identifier;
    };
    struct Int
    {
        int n;
    };
    struct Symbol
    {
        eSymbol symbol;
    };
    using TokenKind = std::variant<Keyword, Identifier, Int, Symbol>;

    struct Token
    {
        TokenKind kind;

        Token() = delete;

        explicit Token(TokenKind kind_) : kind(std::move(kind_)) {}

        bool isSymbol() const
        {
            return kind.index() == 3;
        }

        bool isSymbol(eSymbol symbol_) const
        {
            if (kind.index() == 3)
            {
                auto symbol = std::get<Symbol>(kind);
                return symbol.symbol == symbol_;
            }
            return false;
        }

        bool isInt() const
        {
            return kind.index() == 2;
        }

        bool isIdentifier() const
        {
            return kind.index() == 1;
        }

        bool isKeyword() const
        {
            return kind.index() == 0;
        }

        bool isKeyword(eKeyword keyword_) const
        {
            if (kind.index() == 0)
            {
                auto keyword = std::get<Keyword>(kind);
                return keyword.keyword == keyword_;
            }
            return false;
        }
    };
}

#endif //BCCC_TOKEN_H

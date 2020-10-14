#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_set>
#include <utility>

namespace bccc
{
    struct Token
    {
        enum class Type
        {
            Delimiter,
            Keyword,
            Identifier,
            IntLiteral,
            HexLiteral,
            OctLiteral
        };
        Type type;
        std::string value;

        Token() = delete;

        Token(std::string tokenValue_, Type tokenType_) : value(std::move(tokenValue_)), type(tokenType_)
        { /* empty */
        }

        Token(const Token &token) = default;
    };
} // namespace bccc

#endif
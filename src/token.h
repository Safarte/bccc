#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_set>

namespace bccc
{
    struct Token
    {
        enum class Type
        {
            Delimiter,
            Keyword,
            Identifier,
            Literal
        };
        Type type;
        std::string value;

        Token() = delete;

        Token(const std::string &tokenValue_, Type tokenType_) : value(tokenValue_), type(tokenType_)
        { /* empty */
        }

        Token(const Token &token) : value(token.value), type(token.type)
        { /* empty */
        }
    };

    static const std::unordered_set<char> delims = {'{', '}', '(', ')', ';'};
    static const std::unordered_set<char> whitespace = {'\n', '\r', '\t', ' '};
} // namespace bccc

#endif
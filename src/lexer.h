#ifndef LEXER_H
#define LEXER_H

#include <deque>
#include <vector>
#include <string>
#include <unordered_set>
#include <regex>
#include <iostream>

using LineNo = unsigned int;

namespace bccc
{
    struct Token
    {
        enum class Type
        {
            LeftParen,
            RightParen,
            LeftCurly,
            RightCurly,
            Keyword,
            Semicolon,
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

    class Classifier
    {
    public:
        Classifier(
                Token::Type ttype,
                std::string name,
                std::regex regex) : m_type{ttype},
                                    m_name{std::move(name)},
                                    m_regex{std::move(regex)}
        {
        }

        bool among(const std::string &word) const { return std::regex_match(word, m_regex); }

        Token::Type type() const { return m_type; }

        std::string name() const { return m_name; }

    private:
        Token::Type m_type;
        std::string m_name;
        std::regex m_regex;
    };

    std::vector<Classifier> getClassifiers();

    std::deque<Token> Tokenize(const std::string &filename);

    Token getToken(const std::string &buffer, LineNo line);
}

#endif

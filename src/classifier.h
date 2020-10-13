#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "token.h"

#include <regex>
#include <string>
#include <vector>

namespace bccc
{
    class Classifier
    {
    public:
        Classifier(
            Token::Type ttype,
            std::string name,
            std::regex regex) : m_type{ttype},
                                m_name{name},
                                m_regex{regex}
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

    static const std::vector<Classifier> classifiers {
        {Token::Type::Keyword, "Keyword", std::regex("int|return")},
        {Token::Type::Delimiter, "Delimiter", std::regex(R"({|}|\(|\)|;)", std::regex_constants::basic)},
        {Token::Type::Identifier, "Identifier", std::regex("[a-zA-Z][a-zA-Z0-9_]*", std::regex_constants::basic)},
        {Token::Type::Literal, "Literal", std::regex("[1-9][0-9]*|0[0-7]+|0x[0-9a-f]+", std::regex_constants::extended)},
    };
} // namespace bccc

#endif
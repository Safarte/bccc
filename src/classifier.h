#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "token.h"

#include <regex>
#include <string>
#include <utility>
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
} // namespace bccc

#endif
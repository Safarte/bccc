#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "token.h"

#include <string>
#include <unordered_set>
#include <vector>

namespace bccc
{
    class Classifier
    {
    public:
        Classifier(Token::Type ttype, std::string name, std::unordered_set<std::string> tokens) : m_type{ttype},
                                                                                                  m_name{name},
                                                                                                  m_tokens{tokens}
        {
        }
        bool among(const std::string &word) const { return m_tokens.find(word) != m_tokens.end(); }
        Token::Type type() const { return m_type; }
        std::string name() const { return m_name; }

    private:
        Token::Type m_type;
        std::string m_name;
        std::unordered_set<std::string> m_tokens;
    };

    static const std::vector<Classifier> classifiers{
        {Token::Type::Keyword, "Keyword", {"int", "return"}},
        {Token::Type::Delimiter, "Delimiter", {"{", "}", "(", ")", ";"}},
        {Token::Type::Literal, "Literal", {""}},
        {Token::Type::Identifier, "Identifier", {""}},
    };
} // namespace bccc

#endif
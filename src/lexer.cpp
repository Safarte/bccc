#include "lexer.h"

#include <fstream>
#include <iostream>
#include <deque>
#include <vector>

namespace bccc
{
    std::vector<Classifier> getClassifiers()
    {
        std::vector<Classifier> classifiers{
                {Token::Type::Keyword,    "Keyword",    std::regex("int|return")},
                {Token::Type::LeftParen,  "LeftParen",  std::regex(R"(\()")},
                {Token::Type::RightParen,  "RightParen",  std::regex(R"(\))")},
                {Token::Type::LeftCurly,  "LeftCurly",  std::regex(R"(\{)")},
                {Token::Type::RightCurly,  "RightCurly",  std::regex(R"(\})")},
                {Token::Type::Semicolon,  "Semicolon",  std::regex(";")},
                {Token::Type::Identifier, "Identifier", std::regex("[a-zA-Z][a-zA-Z0-9_]*")},
                {Token::Type::IntLiteral, "IntLiteral", std::regex("0|[1-9][0-9]*")},
                {Token::Type::HexLiteral, "HexLiteral", std::regex("0x[0-9a-f]+")},
                {Token::Type::OctLiteral, "OctLiteral", std::regex("0[0-7]+")},
        };
        return classifiers;
    }

    Token getToken(const std::string &buffer, LineNo line)
    {
        for (const auto &thing : bccc::getClassifiers())
        {
            if (thing.among(buffer))
            {
                return Token(buffer, thing.type());
            }
        }

        /* No support for comments yet*/

        /* Will only reach here if all else fails, invalid string */

        std::cout << "Invalid expression at line number " << line << " : " << buffer;
        std::cin.get();
        exit(0);
    }

    std::deque<Token> Tokenize(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::in);

        if (file.fail())
        {
            std::cout << "Unable to open file!\n";
            std::cin.get();
            exit(0);
        }

        LineNo line = 1;
        std::string buffer;

        char ch;
        std::deque<Token> tokens;
        const std::unordered_set<char> delims = {'{', '}', '(', ')', ';'};
        const std::unordered_set<char> whitespace = {'\n', '\r', '\t', ' '};

        while (file >> std::noskipws >> ch)
        {
            if (ch == '\n' || ch == '\r')
                line += 1;

            if (delims.find(ch) != delims.end())
            {
                if (!buffer.empty())
                {
                    tokens.push_back(getToken(buffer, line));
                    buffer = "";
                }
                tokens.emplace_back(getToken(std::string(1, ch), line));
                continue;
            } else if (whitespace.find(ch) != whitespace.end())
            {
                if (!buffer.empty())
                {
                    tokens.push_back(getToken(buffer, line));
                    buffer = "";
                }
                continue;
            } else
            {
                buffer += ch;
            }
        }

        return tokens;
    }
} // namespace bccc
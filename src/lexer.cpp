#include "lexer.h"
#include "token.h"

#include <fstream>
#include <iostream>
#include <deque>
#include <regex>
#include <unordered_set>

namespace bccc
{
    Token getToken(const std::string &buffer, LineNo line)
    {
        if (buffer == "int")
        {
            return Token(Keyword{eKeyword::Int});
        }
        if (buffer == "return")
        {
            return Token(Keyword{eKeyword::Return});
        }
        if (buffer == "(")
        {
            return Token(Symbol{eSymbol::OpeningParen});
        }
        if (buffer == ")")
        {
            return Token(Symbol{eSymbol::ClosingParen});
        }
        if (buffer == "{")
        {
            return Token(Symbol{eSymbol::OpeningBrace});
        }
        if (buffer == "}")
        {
            return Token(Symbol{eSymbol::ClosingBrace});
        }
        if (buffer == ";")
        {
            return Token(Symbol{eSymbol::Semicolon});
        }
        if (buffer == "+")
        {
            return Token(Symbol{eSymbol::Add});
        }
        if (buffer == "-")
        {
            return Token(Symbol{eSymbol::Sub});
        }
        if (buffer == "*")
        {
            return Token(Symbol{eSymbol::Asterisk});
        }
        if (buffer == "/")
        {
            return Token(Symbol{eSymbol::Div});
        }
        if (buffer == "~")
        {
            return Token(Symbol{eSymbol::BitwiseNot});
        }
        if (buffer == "!")
        {
            return Token(Symbol{eSymbol::LogicalNot});
        }
        if (std::regex_match(buffer, std::regex("[a-zA-Z][a-zA-Z0-9_]*")))
        {
            return Token(Identifier{buffer});
        }
        if (std::regex_match(buffer, std::regex("0|[1-9][0-9]*|0x[0-9a-f]+|0[0-7]+")))
        {
            return Token(Int{std::stoi(buffer, nullptr, 0)});
        }

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
        const std::unordered_set<char> operators = {'+', '-', '*', '/', '~', '!'};

        while (file >> std::noskipws >> ch)
        {
            if (ch == '\n' || ch == '\r')
                line += 1;

            if (delims.find(ch) != delims.end() || operators.find(ch) != operators.end())
            {
                if (!buffer.empty())
                {
                    tokens.push_back(getToken(buffer, line));
                    buffer = "";
                }
                tokens.push_back(getToken(std::string(1, ch), line));
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
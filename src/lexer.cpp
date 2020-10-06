#include "lexer.h"
#include "classifier.h"
#include "token.h"

#include <fstream>
#include <iostream>
#include <regex>

namespace bccc
{
    Token getToken(std::string buffer, LineNo line)
    {
        for (const auto &thing : classifiers)
        {
            if (thing.among(buffer))
            {
                return Token(buffer, thing.type());
            }
        }

        std::regex iden("[a-zA-Z][a-zA-Z0-9_]*");
        std::regex lit("^([1-9][0-9]*|0[0-7]+|0x[0-9a-f]+)$");
        //std::regex;

        if (std::regex_match(buffer, iden))
        {
            return Token(buffer, Token::Type::Identifier);
        }
        if (std::regex_match(buffer, lit))
        {
            return Token(buffer, Token::Type::Literal);
        }

        /* No support for comments yet*/

        /* Will only reach here if all else fails, invalid string */

        std::cout << "Invalid expression at line number " << line << " : " << buffer;
        std::cin.get();
        exit(0);
    }

    std::vector<Token> Tokenize(std::string filename)
    {
        std::ifstream file(filename, std::ios::in);

        if (file.fail())
        {
            std::cout << "Unable to open file!\n";
            std::cin.get();
            exit(0);
        }

        LineNo line = 1;
        std::string buffer = "";

        char ch;
        std::vector<Token> tokens;

        while (file >> std::noskipws >> ch)
        {
            if (ch == '\n' || ch == '\r')
                line += 1;

            if (delims.find(ch) != delims.end())
            {
                if (buffer != "")
                {
                    tokens.push_back(getToken(buffer, line));
                    buffer = "";
                }
                tokens.push_back(Token(std::string(1, ch), Token::Type::Delimiter));
                continue;
            }

            else if (whitespace.find(ch) != whitespace.end())
            {
                if (buffer != "")
                {
                    tokens.push_back(getToken(buffer, line));
                    buffer = "";
                }
                continue;
            }
            else
            {
                buffer += ch;
            }
        }

        //std::cout << line << "\n";

        return tokens;
    }
} // namespace bccc
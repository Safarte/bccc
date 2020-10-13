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

        return tokens;
    }
} // namespace bccc
#include "lexer.h"
#include "parser.h"

#include <deque>
#include <iostream>
#include <string>

namespace bccc
{
    std::pair<Expression, std::deque<Token>> parseExpression(std::deque<Token> &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        switch (token.type)
        {
            case Token::Type::HexLiteral:
            case Token::Type::OctLiteral:
            case Token::Type::IntLiteral:
            {
                Constant constant(token.value);
                return std::pair(Expression(constant), tokens);
            }
            default:
                throw std::runtime_error("Invalid expression");
        }
    }

    std::pair<Statement, std::deque<Token>> parseStatement(std::deque<Token> &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        if (!(token.type == Token::Type::Keyword && token.value == "return"))
        {
            throw std::runtime_error("'return' keyword expected");
        }

        auto res = parseExpression(tokens);
        auto expression = res.first;
        tokens = res.second;

        Return returnNode(expression);
        Statement statement(returnNode);

        token = tokens.front();
        tokens.pop_front();

        if (token.type != Token::Type::Semicolon)
        {
            throw std::runtime_error("Missing semicolon");
        }

        return std::pair(statement, tokens);
    }

    std::pair<Function, std::deque<Token>> parseFunction(std::deque<Token> &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        if (!(token.type == Token::Type::Keyword && token.value == "int"))
        {
            throw std::runtime_error("'int' keyword expected");
        }

        token = tokens.front();
        tokens.pop_front();

        if (token.type != Token::Type::Identifier)
        {
            throw std::runtime_error("Missing function name identifier");
        }

        std::string name = token.value;

        token = tokens.front();
        tokens.pop_front();

        if (token.type != Token::Type::LeftParen)
        {
            throw std::runtime_error("Missing opening parenthesis");
        }

        token = tokens.front();
        tokens.pop_front();

        if (token.type != Token::Type::RightParen)
        {
            throw std::runtime_error("Missing closing parenthesis");
        }

        token = tokens.front();
        tokens.pop_front();

        if (token.type != Token::Type::LeftCurly)
        {
            throw std::runtime_error("Missing opening braces");
        }

        auto res = parseStatement(tokens);
        auto statement = res.first;
        tokens = res.second;

        token = tokens.front();
        tokens.pop_front();

        if (token.type != Token::Type::RightCurly)
        {
            throw std::runtime_error("Missing closing braces");
        }

        Function function(name, statement);
        return std::pair(function, tokens);
    }

    Program parseProgram(std::deque<Token> &tokens_)
    {
        auto tokens = tokens_;
        auto res = parseFunction(tokens);
        auto function = res.first;
        tokens = res.second;

        if (!tokens.empty())
        {
            throw std::runtime_error("Unexpected tokens at end of program");
        }

        Program program(function);
        return program;
    }

    std::ostream &operator<<(std::ostream &os, Constant &constant_)
    {
        std::stringstream ss;
        ss << "Int<" << constant_.value << ">";
        return os << ss.str();
    }
    std::ostream &operator<<(std::ostream &os, Return &return_)
    {
        std::stringstream ss;
        ss << "RETURN " << return_.expression;
        return os << ss.str();
    }
    std::ostream &operator<<(std::ostream &os, Expression &expression_)
    {
        return os << expression_.body;
    }
    std::ostream &operator<<(std::ostream &os, Statement &statement_)
    {
        return os << statement_.body;
    }
    std::ostream &operator<<(std::ostream &os, Program &program_)
    {
        return os << program_.body;
    }
    std::ostream &operator<<(std::ostream &os, Function &function_)
    {
        std::stringstream ss;
        ss << "FUN " << function_.sName << ":\n\tbody:\n\t\t" << function_.body;
        return os << ss.str();
    }
}
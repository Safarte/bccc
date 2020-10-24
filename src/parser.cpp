#include "token.h"
#include "parser.h"
#include "ast.h"

#include <deque>
#include <string>
#include <utility>
#include <memory>

namespace bccc
{
    std::pair<AST, Tokens> parseExpression(Tokens &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        if (token.isInt())
        {
            auto value = std::get<Int>(token.kind);
            auto node = AST(Int{value.n});
            return {node, tokens};
        }
        if (token.isSymbol())  // Token is a Symbol
        {
            auto symbol = std::get<Symbol>(token.kind);
            eUnaryOp op;
            switch (symbol.symbol)
            {
                case eSymbol::Sub:
                    op = eUnaryOp::Minus;
                    break;
                case eSymbol::LogicalNot:
                    op = eUnaryOp::LogicalNot;
                    break;
                case eSymbol::BitwiseNot:
                    op = eUnaryOp::BitwiseNot;
                    break;
                default:
                    throw std::runtime_error("Unknown symbol");
            }

            auto[expr, newTokens] = parseExpression(tokens);
            tokens = newTokens;
            auto node = AST(UnaryOp{op, std::make_unique<AST>(expr)});

            return {node, tokens};
        }
        throw std::runtime_error("Invalid expression");
    }

    std::pair<AST, Tokens> parseStatement(Tokens &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        if (token.isKeyword(eKeyword::Return))  // Token is a Keyword
        {
            auto[expr, newTokens] = parseExpression(tokens);
            tokens = newTokens;

            token = tokens.front();
            tokens.pop_front();

            if (token.isSymbol())
            {
                auto symbol = std::get<Symbol>(token.kind);
                if (symbol.symbol == eSymbol::Semicolon)
                {
                    auto node = AST(Return{std::make_unique<AST>(expr)});
                    return {node, tokens};
                }
            }
            throw std::runtime_error("Missing semicolon");
        }
        throw std::runtime_error("'return' keyword expected");
    }

    std::pair<std::vector<Identifier>, Tokens> parseArguments(Tokens &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        if (token.isSymbol(eSymbol::OpeningParen))
        {
            token = tokens.front();
            tokens.pop_front();

            if (token.isSymbol(eSymbol::ClosingParen))
            {
                std::vector<Identifier> args;
                return {args, tokens};
            }
            throw std::runtime_error("Missing closing parenthesis");
        }
        throw std::runtime_error("Missing opening parenthesis");
    }

    std::pair<AST, Tokens> parseFunction(Tokens &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        if (token.isKeyword(eKeyword::Int))
        {
            token = tokens.front();
            tokens.pop_front();

            if (!token.isIdentifier())
            {
                throw std::runtime_error("Missing function name identifier");
            }

            auto identifier = std::get<Identifier>(token.kind);
            std::string name = identifier.identifier;

            auto[args, newTokens] = parseArguments(tokens);
            tokens = newTokens;

            token = tokens.front();
            tokens.pop_front();

            if (token.isSymbol(eSymbol::OpeningBrace))
            {
                auto[statement, newTokens] = parseStatement(tokens);
                tokens = newTokens;

                token = tokens.front();
                tokens.pop_front();

                if (token.isSymbol(eSymbol::ClosingBrace))
                {
                    auto node = AST(FuncDef{name, std::make_unique<AST>(statement)});
                    return { node, tokens };
                }
                throw std::runtime_error("Missing closing braces");
            }
            throw std::runtime_error("Missing opening braces");
        }
        throw std::runtime_error("'int' keyword expected");
    }

    AST parseProgram(Tokens &tokens_)
    {
        auto tokens = tokens_;
        auto[ast, newTokens] = parseFunction(tokens);
        tokens = newTokens;

        if (!tokens.empty())
        {
            throw std::runtime_error("Unexpected tokens at end of program");
        }

        return ast;
    }
}
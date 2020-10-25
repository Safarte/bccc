#include "token.h"
#include "parser.h"
#include "ast.h"

#include <deque>
#include <string>
#include <utility>
#include <memory>
#include <iostream>

namespace bccc
{
    std::pair<AST, Tokens> parseFactor(Tokens &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        if (token.isSymbol(eSymbol::OpeningParen))
        {
            auto[expr, newTokens] = parseExpression(tokens);
            tokens = newTokens;

            token = tokens.front();
            tokens.pop_front();

            if (!token.isSymbol(eSymbol::ClosingParen))
            {
                throw std::runtime_error("Missing closing parenthesis");
            }

            return {expr, tokens};
        }
        if (token.isUnaryOp())
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
                    throw std::runtime_error("Unknown unary operator");
            }

            auto[factor, newTokens] = parseFactor(tokens);
            tokens = newTokens;

            auto node = AST(UnaryOp{op, std::make_unique<AST>(factor)});
            return {node, tokens};
        }
        if (token.isInt())
        {
            auto value = std::get<Int>(token.kind);
            auto node = AST(Int{value.n});
            return {node, tokens};
        }
        throw std::runtime_error("Invalid expression");
    }

    std::pair<AST, Tokens> parseTerm(Tokens &tokens_)
    {
        auto[factor, tokens] = parseFactor(tokens_);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::Asterisk) || token.isSymbol(eSymbol::Div))
        {
            tokens.pop_front();

            auto symbol = std::get<Symbol>(token.kind);
            eBinaryOp op;
            switch (symbol.symbol)
            {
                case eSymbol::Asterisk:
                    op = eBinaryOp::Mul;
                    break;
                case eSymbol::Div:
                    op = eBinaryOp::Div;
                    break;
                default:
                    throw std::runtime_error("Unknown binary operator");
            }

            auto[nextFactor, newTokens] = parseFactor(tokens);
            tokens = newTokens;

            factor.setKind(BinaryOp{op, std::make_unique<AST>(factor), std::make_unique<AST>(nextFactor)});

            token = tokens.front();
        }

        return {factor, tokens};
    }

    std::pair<AST, Tokens> parseExpression(Tokens &tokens_)
    {
        auto[term, tokens] = parseTerm(tokens_);

        auto token = tokens.front();


        while (token.isSymbol(eSymbol::Add) || token.isSymbol(eSymbol::Sub))
        {
            tokens.pop_front();

            auto symbol = std::get<Symbol>(token.kind);
            eBinaryOp op;
            switch (symbol.symbol)
            {
                case eSymbol::Add:
                    op = eBinaryOp::Add;
                    break;
                case eSymbol::Sub:
                    op = eBinaryOp::Sub;
                    break;
                default:
                    throw std::runtime_error("Unknown binary operator");
            }

            auto[nextTerm, newTokens] = parseTerm(tokens);
            tokens = newTokens;

            term.setKind(BinaryOp{op, std::make_unique<AST>(term), std::make_unique<AST>(nextTerm)});

            token = tokens.front();
        }

        return {term, tokens};
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

            if (token.isSymbol(eSymbol::Semicolon))
            {
                auto node = AST(Return{std::make_unique<AST>(expr)});
                return {node, tokens};
            }
            std::cout << "Missing semicolon";
            exit(0);
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
                    return {node, tokens};
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
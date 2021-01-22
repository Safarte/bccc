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
            auto [expr, newTokens] = parseExpression(tokens);
            tokens = newTokens;

            token = tokens.front();
            tokens.pop_front();

            if (!token.isSymbol(eSymbol::ClosingParen))
            {
                std::cout << "Missing closing parenthesis";
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
                std::cout << "Unknown unary operator";
                exit(0);
            }

            auto [factor, newTokens] = parseFactor(tokens);
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
        std::cout << "Invalid expression";
        exit(0);
    }

    std::pair<AST, Tokens> parseTerm(Tokens &tokens_)
    {
        auto [factor, tokens] = parseFactor(tokens_);

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
                std::cout << "Unknown binary operator";
                exit(0);
            }

            auto [nextFactor, newTokens] = parseFactor(tokens);
            tokens = newTokens;

            factor.setKind(BinaryOp{op, std::make_unique<AST>(factor), std::make_unique<AST>(nextFactor)});

            token = tokens.front();
        }

        return {factor, tokens};
    }

    std::pair<AST, Tokens> parseAddExpr(Tokens &tokens_)
    {
        auto [term, tokens] = parseTerm(tokens_);

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
                std::cout << "Unknown binary operator";
                exit(0);
            }

            auto [nextTerm, newTokens] = parseTerm(tokens);
            tokens = newTokens;

            term.setKind(BinaryOp{op, std::make_unique<AST>(term), std::make_unique<AST>(nextTerm)});

            token = tokens.front();
        }

        return {term, tokens};
    }

    std::pair<AST, Tokens> parseRelExpr(Tokens &tokens_)
    {
        auto [addExpr, tokens] = parseAddExpr(tokens_);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::Lt) || token.isSymbol(eSymbol::Le) || token.isSymbol(eSymbol::Gt) ||
               token.isSymbol(eSymbol::Ge))
        {
            tokens.pop_front();

            auto symbol = std::get<Symbol>(token.kind);
            eBinaryOp op;
            switch (symbol.symbol)
            {
            case eSymbol::Lt:
                op = eBinaryOp::Lt;
                break;
            case eSymbol::Le:
                op = eBinaryOp::Le;
                break;
            case eSymbol::Gt:
                op = eBinaryOp::Gt;
                break;
            case eSymbol::Ge:
                op = eBinaryOp::Ge;
                break;
            default:
                std::cout << "Unknown binary operator";
                exit(0);
            }

            auto [nextAddExpr, newTokens] = parseAddExpr(tokens);
            tokens = newTokens;

            addExpr.setKind(BinaryOp{op, std::make_unique<AST>(addExpr), std::make_unique<AST>(nextAddExpr)});

            token = tokens.front();
        }

        return {addExpr, tokens};
    }

    std::pair<AST, Tokens> parseEqExpr(Tokens &tokens_)
    {
        auto [relExpr, tokens] = parseRelExpr(tokens_);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::Eq) || token.isSymbol(eSymbol::Ne))
        {
            tokens.pop_front();

            auto symbol = std::get<Symbol>(token.kind);
            eBinaryOp op;
            switch (symbol.symbol)
            {
            case eSymbol::Eq:
                op = eBinaryOp::Eq;
                break;
            case eSymbol::Ne:
                op = eBinaryOp::Ne;
                break;
            default:
                std::cout << "Unknown binary operator";
                exit(0);
            }

            auto [nextRelExpr, newTokens] = parseRelExpr(tokens);
            tokens = newTokens;

            relExpr.setKind(BinaryOp{op, std::make_unique<AST>(relExpr), std::make_unique<AST>(nextRelExpr)});

            token = tokens.front();
        }

        return {relExpr, tokens};
    }

    std::pair<AST, Tokens> parseAndExpr(Tokens &tokens_)
    {
        auto [eqExpr, tokens] = parseEqExpr(tokens_);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::LogicalAnd))
        {
            tokens.pop_front();

            auto op = eBinaryOp::LAnd;

            auto [nextEqExpr, newTokens] = parseEqExpr(tokens);
            tokens = newTokens;

            eqExpr.setKind(BinaryOp{op, std::make_unique<AST>(eqExpr), std::make_unique<AST>(nextEqExpr)});

            token = tokens.front();
        }

        return {eqExpr, tokens};
    }

    std::pair<AST, Tokens> parseExpression(Tokens &tokens_)
    {
        auto [andExpr, tokens] = parseAndExpr(tokens_);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::LogicalOr))
        {
            tokens.pop_front();

            auto op = eBinaryOp::LOr;

            auto [nextAndExpr, newTokens] = parseAndExpr(tokens);
            tokens = newTokens;

            andExpr.setKind(BinaryOp{op, std::make_unique<AST>(andExpr), std::make_unique<AST>(nextAndExpr)});

            token = tokens.front();
        }

        return {andExpr, tokens};
    }

    std::pair<AST, Tokens> parseStatement(Tokens &tokens_)
    {
        auto tokens = tokens_;
        Token token = tokens.front();
        tokens.pop_front();

        if (token.isKeyword(eKeyword::Return)) // Token is a Keyword
        {
            auto [expr, newTokens] = parseExpression(tokens);
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
        std::cout << "'return' keyword expected";
        exit(0);
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
            std::cout << "Missing closing parenthesis";
            exit(0);
        }
        std::cout << "Missing opening parenthesis";
        exit(0);
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
                std::cout << "Missing function name identifier";
                exit(0);
            }

            auto identifier = std::get<Identifier>(token.kind);
            std::string name = identifier.identifier;

            auto [args, newTokens] = parseArguments(tokens);
            tokens = newTokens;

            token = tokens.front();
            tokens.pop_front();

            if (token.isSymbol(eSymbol::OpeningBrace))
            {
                auto [statement, newTokens] = parseStatement(tokens);
                tokens = newTokens;

                token = tokens.front();
                tokens.pop_front();

                if (token.isSymbol(eSymbol::ClosingBrace))
                {
                    auto node = AST(FuncDef{name, std::make_unique<AST>(statement)});
                    return {node, tokens};
                }
                std::cout << "Missing closing braces";
                exit(0);
            }
            std::cout << "Missing opening braces";
            exit(0);
        }
        std::cout << "'int' keyword expected";
        exit(0);
    }

    AST parseProgram(Tokens &tokens_)
    {
        auto tokens = tokens_;
        auto [ast, newTokens] = parseFunction(tokens);
        tokens = newTokens;

        if (!tokens.empty())
        {
            std::cout << "Unexpected tokens at end of program";
            exit(0);
        }

        return ast;
    }
} // namespace bccc
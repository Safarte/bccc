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
    AST* parseFactor(Tokens &tokens)
    {
        Token token = tokens.front();
        tokens.pop_front();

        if (token.isSymbol(eSymbol::OpeningParen))
        {
            auto expr = parseExpression(tokens);

            token = tokens.front();
            tokens.pop_front();

            if (!token.isSymbol(eSymbol::ClosingParen))
            {
                std::cout << "Missing closing parenthesis";
            }

            return expr;
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

            auto factor = parseFactor(tokens);

            AST *node = new UnaryOp{op, factor};
            return node;
        }
        if (token.isInt())
        {
            auto value = std::get<Int>(token.kind);
            AST *node = new IntLit{value.n};
            return node;
        }
        if (token.isIdentifier())
        {
            auto name = std::get<Identifier>(token.kind);
            AST *node = new Var{name.identifier};
            return node;
        }
        std::cout << token << std::endl;
        std::cout << "Invalid expression";
        exit(0);
    }

    AST* parseTerm(Tokens &tokens)
    {
        auto factor = parseFactor(tokens);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::Asterisk) || token.isSymbol(eSymbol::Div) || token.isSymbol(eSymbol::Mod))
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
                case eSymbol::Mod:
                    op = eBinaryOp::Mod;
                    break;
                default:
                    std::cout << "Unknown binary operator";
                    exit(0);
            }

            auto nextFactor = parseFactor(tokens);

            AST *node = new BinaryOp{op, factor, nextFactor};
            factor = node;

            token = tokens.front();
        }

        return factor;
    }

    AST* parseAddExpr(Tokens &tokens)
    {
        auto term = parseTerm(tokens);

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

            auto nextTerm = parseTerm(tokens);

            AST *node = new BinaryOp{op, term, nextTerm};
            term = node;

            token = tokens.front();
        }

        return term;
    }

    AST* parseShiftExpr(Tokens &tokens)
    {
        auto addExpr = parseAddExpr(tokens);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::Shl) || token.isSymbol(eSymbol::Shr))
        {
            tokens.pop_front();

            auto symbol = std::get<Symbol>(token.kind);
            eBinaryOp op;
            switch (symbol.symbol)
            {
                case eSymbol::Shl:
                    op = eBinaryOp::Shl;
                    break;
                case eSymbol::Shr:
                    op = eBinaryOp::Shr;
                    break;
                default:
                    std::cout << "Unknown binary operator";
                    exit(0);
            }

            auto nextAddExpr = parseAddExpr(tokens);

            AST *node = new BinaryOp{op, addExpr, nextAddExpr};
            addExpr = node;

            token = tokens.front();
        }

        return addExpr;
    }

    AST* parseRelExpr(Tokens &tokens)
    {
        auto shiftExpr = parseShiftExpr(tokens);

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

            auto nextShiftExpr = parseShiftExpr(tokens);

            AST *node = new BinaryOp{op, shiftExpr, nextShiftExpr};
            shiftExpr = node;

            token = tokens.front();
        }

        return shiftExpr;
    }

    AST* parseEqExpr(Tokens &tokens)
    {
        auto relExpr = parseRelExpr(tokens);

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

            auto nextRelExpr = parseRelExpr(tokens);

            AST *node = new BinaryOp{op, relExpr, nextRelExpr};
            relExpr = node;

            token = tokens.front();
        }

        return relExpr;
    }

    AST* parseAndExpr(Tokens &tokens)
    {
        auto eqExpr = parseEqExpr(tokens);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::And))
        {
            tokens.pop_front();

            auto op = eBinaryOp::And;

            auto nextEqExpr = parseEqExpr(tokens);

            AST *node = new BinaryOp{op, eqExpr, nextEqExpr};
            eqExpr = node;

            token = tokens.front();
        }

        return eqExpr;
    }

    AST* parseXorExpr(Tokens &tokens)
    {
        auto andExpr = parseAndExpr(tokens);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::Xor))
        {
            tokens.pop_front();

            auto op = eBinaryOp::Xor;

            auto nextAndExpr = parseAndExpr(tokens);

            AST *node = new BinaryOp{op, andExpr, nextAndExpr};
            andExpr = node;

            token = tokens.front();
        }

        return andExpr;
    }

    AST* parseOrExpr(Tokens &tokens)
    {
        auto xorExpr = parseXorExpr(tokens);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::Or))
        {
            tokens.pop_front();

            auto op = eBinaryOp::Or;

            auto nextXorExpr = parseXorExpr(tokens);

            AST *node = new BinaryOp{op, xorExpr, nextXorExpr};
            xorExpr = node;

            token = tokens.front();
        }

        return xorExpr;
    }

    AST* parseLAndExpr(Tokens &tokens)
    {
        auto orExpr = parseOrExpr(tokens);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::LogicalAnd))
        {
            tokens.pop_front();

            auto op = eBinaryOp::LAnd;

            auto nextOrExpr = parseOrExpr(tokens);

            AST *node = new BinaryOp{op, orExpr, nextOrExpr};
            orExpr = node;

            token = tokens.front();
        }

        return orExpr;
    }

    AST* parseLOrExpr(Tokens &tokens)
    {
        auto lAndExpr = parseLAndExpr(tokens);

        auto token = tokens.front();

        while (token.isSymbol(eSymbol::LogicalOr))
        {
            tokens.pop_front();

            auto op = eBinaryOp::LOr;

            auto nextLAndExpr = parseLAndExpr(tokens);

            AST *node = new BinaryOp{op, lAndExpr, nextLAndExpr};
            lAndExpr = node;

            token = tokens.front();
        }

        return lAndExpr;
    }

    AST* parseExpression(Tokens &tokens)
    {
        Token token = tokens.front();

        if (token.isIdentifier())
        {
            auto tempTokens = tokens;
            tempTokens.pop_front();

            auto name = std::get<Identifier>(token.kind);

            token = tempTokens.front();
            tempTokens.pop_front();

            if (token.isSymbol(eSymbol::Assign))
            {
                tokens = tempTokens;
                auto expr = parseExpression(tokens);
                AST* node = new Assign{name.identifier, expr};
                return node;
            }
        }
        return parseLOrExpr(tokens);
    }

    AST* parseStatement(Tokens &tokens)
    {
        Token token = tokens.front();

        if (token.isKeyword(eKeyword::Return)) // Token is a Keyword
        {
            tokens.pop_front();

            auto expr = parseExpression(tokens);

            token = tokens.front();
            tokens.pop_front();

            if (token.isSymbol(eSymbol::Semicolon))
            {
                AST* node = new Return{expr};
                return node;
            }
            std::cout << "Missing semicolon";
            exit(0);
        }
        if (token.isKeyword(eKeyword::Int))
        {
            tokens.pop_front();

            token = tokens.front();
            tokens.pop_front();

            if (token.isIdentifier())
            {
                auto name = std::get<Identifier>(token.kind);

                token = tokens.front();
                tokens.pop_front();

                if (token.isSymbol(eSymbol::Semicolon))
                {
                    AST* node = new Declare{name.identifier};
                    return node;
                }
                if (token.isSymbol(eSymbol::Assign))
                {
                    auto expr = parseExpression(tokens);

                    token = tokens.front();
                    tokens.pop_front();

                    if (token.isSymbol(eSymbol::Semicolon))
                    {
                        AST *node = new Declare{name.identifier, expr};
                        return node;
                    }
                }
            }
        }

        auto expr = parseExpression(tokens);

        token = tokens.front();
        tokens.pop_front();

        if (token.isSymbol(eSymbol::Semicolon))
        {
            return expr;
        }

        std::cout << "'return', expression or assignment expected";
        exit(0);
    }

    std::vector<Identifier> parseArguments(Tokens &tokens)
    {
        Token token = tokens.front();
        tokens.pop_front();

        if (token.isSymbol(eSymbol::OpeningParen))
        {
            token = tokens.front();
            tokens.pop_front();

            if (token.isSymbol(eSymbol::ClosingParen))
            {
                std::vector<Identifier> args;
                return args;
            }
            std::cout << "Missing closing parenthesis";
            exit(0);
        }
        std::cout << "Missing opening parenthesis";
        exit(0);
    }

    AST* parseFunction(Tokens &tokens)
    {
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

            auto args = parseArguments(tokens);

            token = tokens.front();
            tokens.pop_front();

            if (token.isSymbol(eSymbol::OpeningBrace))
            {
                std::vector<AST *> body;

                while (!(tokens.empty() || tokens.front().isSymbol(eSymbol::ClosingBrace)))
                {
                    auto statement = parseStatement(tokens);

                    body.emplace_back(statement);
                }

                if (!tokens.empty())
                {
                    token = tokens.front();
                    tokens.pop_front();

                    if (token.isSymbol(eSymbol::ClosingBrace))
                    {
                        AST *node = new FuncDef{name, body};
                        return node;
                    }
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

    AST* parseProgram(Tokens &tokens_)
    {
        auto tokens = tokens_;
        auto ast = parseFunction(tokens);

        if (!tokens.empty())
        {
            std::cout << "Unexpected tokens at end of program";
            exit(0);
        }

        return ast;
    }
} // namespace bccc
#include "ast.h"

#include <iostream>
#include <map>
#include <string>

namespace bccc
{
    std::map<int, char> getUnaryOpChar()
    {
        return std::map<int, char>{
            {0, '-'},
            {1, '~'},
            {2, '!'}};
    }

    std::map<int, std::string> getBinaryOpChar()
    {
        return std::map<int, std::string>{
            {0, "+"},
            {1, "-"},
            {2, "*"},
            {3, "/"},
            {4, "&&"},
            {5, "||"},
            {6, "=="},
            {7, "!="},
            {8, "<"},
            {9, "<="},
            {10, ">"},
            {11, ">="},
            {12, "%"},
            {13, "&"},
            {14, "|"},
            {15, "^"},
            {16, "<<"},
            {17, ">>"}};
    }

    std::ostream &operator<<(std::ostream &os, AST &ast)
    {
        auto mUnaryOp = getUnaryOpChar();
        auto mBinaryOp = getBinaryOpChar();

        switch (ast.kind.index())
        {
        case 0: // FuncDef
        {
            auto [name, body] = std::get<FuncDef>(std::move(ast.kind));
            os << "FUN " << name << ":\n";
            os << "\tbody:\n";
            os << "\t\t" << *body << "\n";
            break;
        }
        case 1: // Int
        {
            auto [n] = std::get<Int>(std::move(ast.kind));
            os << "Int<" << n << ">";
            break;
        }
        case 2: // UnaryOp
        {
            auto [op, expr] = std::get<UnaryOp>(std::move(ast.kind));
            os << mUnaryOp[(int)op] << *expr;
            break;
        }
        case 3: // BinaryOp
        {
            auto [op, lExpr, rExpr] = std::get<BinaryOp>(std::move(ast.kind));
            os << "(" << *lExpr << " " << mBinaryOp[(int)op] << " " << *rExpr << ")";
            break;
        }
        case 4: // Return
        {
            auto [expr] = std::get<Return>(std::move(ast.kind));
            os << "RETURN " << *expr;
            break;
        }
        default:
            break;
        }
        return os;
    }

    void AST::setKind(ASTKind kind_)
    {
        kind = std::move(kind_);
    }
} // namespace bccc

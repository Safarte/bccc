#ifndef BCCC_AST_H
#define BCCC_AST_H

#include "token.h"

#include <variant>
#include <string>
#include <memory>
#include <iostream>
#include <map>

namespace bccc
{
    enum class eUnaryOp
    {
        Minus,
        BitwiseNot,
        LogicalNot
    };

    std::map<int, char> getUnaryOpChar();

    enum class eBinaryOp
    {
        Add,
        Sub,
        Mul,
        Div,
        LAnd,
        LOr,
        Eq,
        Ne,
        Lt,
        Le,
        Gt,
        Ge
    };

    std::map<int, std::string> getBinaryOpChar();

    struct AST;

    struct FuncDef
    {
        std::string name;
        std::unique_ptr<AST> body;
    };

    struct UnaryOp
    {
        eUnaryOp op;
        std::unique_ptr<AST> operand;
    };

    struct BinaryOp
    {
        eBinaryOp op;
        std::unique_ptr<AST> leftOperand;
        std::unique_ptr<AST> rightOperand;
    };

    struct Return
    {
        std::unique_ptr<AST> expression;
    };

    using ASTKind = std::variant<FuncDef, Int, UnaryOp, BinaryOp, Return>;

    struct AST
    {
        ASTKind kind;

        AST() = delete;

        explicit AST(ASTKind kind_) : kind(std::move(kind_)) {}

        AST(AST &ast_) : kind(std::move(ast_.kind)) {}

        void setKind(ASTKind kind_);

        bool isFuncDef() const
        {
            return kind.index() == 0;
        }

        bool isInt() const
        {
            return kind.index() == 1;
        }

        bool isUnaryOp() const
        {
            return kind.index() == 2;
        }

        bool isBinaryOp() const
        {
            return kind.index() == 3;
        }

        bool isReturn() const
        {
            return kind.index() == 4;
        }
    };

    std::ostream& operator<<(std::ostream &os, AST &ast);
}

#endif //BCCC_AST_H

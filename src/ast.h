#ifndef BCCC_AST_H
#define BCCC_AST_H

#include "token.h"

#include <variant>
#include <string>
#include <memory>

namespace bccc
{
    enum class eUnaryOp
    {
        Minus,
        BitwiseNot,
        LogicalNot
    };

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
    struct Return
    {
        std::unique_ptr<AST> expression;
    };

    using ASTKind = std::variant<FuncDef, Int, UnaryOp, Return>;

    struct AST
    {
        ASTKind kind;

        AST() = delete;

        explicit AST(ASTKind kind_) : kind(std::move(kind_)) {}

        AST(AST &ast_) : kind(std::move(ast_.kind)) {}

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

        bool isReturn() const
        {
            return kind.index() == 3;
        }
    };
}

#endif //BCCC_AST_H

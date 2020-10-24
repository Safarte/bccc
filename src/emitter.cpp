#include "parser.h"
#include "emitter.h"

#include <string>
#include <sstream>

namespace bccc
{
    std::string emitExpression(AST &expression)
    {
        std::stringstream ss;

        if (expression.isInt())
        {
            auto intKind = std::get<Int>(expression.kind);
            ss << "\tmovl\t$" << intKind.n << ", %eax\n";
        }

        if (expression.isUnaryOp())
        {
            auto[op, operand] = std::get<UnaryOp>(std::move(expression.kind));

            ss << emitExpression(*operand);

            switch (op)
            {
                case eUnaryOp::Minus:
                    ss << "\tneg\t%eax\n";
                    break;
                case eUnaryOp::BitwiseNot:
                    ss << "\tnot\t%eax\n";
                    break;
                case eUnaryOp::LogicalNot:
                    ss << "\tcmpl\t$0, %eax\n";
                    ss << "\tmovl\t$0, %eax\n";
                    ss << "\tsete\t%al\n";
                    break;
            }
        }

        return ss.str();
    }

    std::string emitReturn(AST &ret)
    {
        std::stringstream ss;

        auto[expr] = std::get<Return>(std::move(ret.kind));

        ss << emitExpression(*expr);

        ss << "\tret\n";

        return ss.str();
    }

    std::string emitStatement(AST &statement)
    {
        std::stringstream ss;

        if (statement.isReturn())
        {
            ss << emitReturn(statement);
        }

        return ss.str();
    }

    std::string emitFunction(AST &function)
    {
        std::stringstream ss;

        auto[name, body] = std::get<FuncDef>(std::move(function.kind));

        ss << ".globl " << name << "\n";
        ss << name << ":\n";
        ss << emitStatement(*body);

        return ss.str();
    }

    std::string emitProgram(AST &program)
    {
        std::stringstream ss;

        if (program.isFuncDef())
        {
            ss << emitFunction(program);
        }

        return ss.str();
    }
}

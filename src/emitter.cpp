#include "parser.h"
#include "emitter.h"

#include <string>
#include <sstream>
#include <utility>

namespace bccc
{
    std::pair<std::string, int> emitOperands(AST &op1, AST &op2, int counter)
    {
        std::stringstream ss;

        auto[expr1, counter1] = emitExpression(op1, counter);
        ss << expr1;
        ss << "\tpush\t%rax\n";
        auto[expr2, counter2] = emitExpression(op2, counter1);
        ss << expr2;
        ss << "\tpop\t%rcx\n";

        return { ss.str(), counter2 };
    }

    std::pair<std::string, int> emitComp(AST &op1, AST &op2, int counter)
    {
        std::stringstream ss;

        auto[expr, counter_] = emitOperands(op1, op2, counter);
        ss << expr;
        ss << "\tcmpl\t%eax, %ecx\n";
        ss << "\tmovl\t$0, %eax\n";

        return { ss.str(), counter_ };
    }

    std::pair<std::string, int> emitExpression(AST &expression, int counter)
    {
        std::stringstream ss;
        int counter_ = counter;

        if (expression.isInt())
        {
            auto intKind = std::get<Int>(expression.kind);
            ss << "\tmovl\t$" << intKind.n << ", %eax\n";
        }

        if (expression.isUnaryOp())
        {
            auto[op, operand] = std::get<UnaryOp>(std::move(expression.kind));

            auto[expr, counter1] = emitExpression(*operand, counter);
            counter_ = counter1;
            ss << expr;

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

        if (expression.isBinaryOp())
        {
            auto[op, lOperand, rOperand] = std::get<BinaryOp>(std::move(expression.kind));

            switch(op)
            {
                case eBinaryOp::Add:
                {
                    auto[expr, counter1] = emitOperands(*lOperand, *rOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\taddl\t%ecx, %eax\n";
                    break;
                }
                case eBinaryOp::Sub:
                {
                    auto[expr, counter1] = emitOperands(*rOperand, *lOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\tsubl\t%ecx, %eax\n";
                    break;
                }
                case eBinaryOp::Mul:
                {
                    auto[expr, counter1] = emitOperands(*lOperand, *rOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\timul\t%ecx, %eax\n";
                    break;
                }
                case eBinaryOp::Div:
                {
                    auto[expr, counter1] = emitOperands(*rOperand, *lOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\tcdq\n";
                    ss << "\tidiv\t%ecx\n";
                    break;
                }
                case eBinaryOp::LAnd:
                {
                    auto[expr1, counter1] = emitExpression(*lOperand, counter);
                    ss << expr1;
                    ss << "\tcmpl\t$0, %eax\n";
                    ss << "\tjne\t_" << counter1 << "\n";
                    ss << "\tmovl\t$1, %eax\n";
                    auto[expr2, counter2] = emitExpression(*rOperand, counter1 + 1);
                    ss << "\tjmp\t_" << counter2 << "\n";
                    ss << "_" << counter1 << ":\n";
                    ss << expr2;
                    ss << "\tcmpl\t$0, %eax\n";
                    ss << "\tmovl\t$0, %eax\n";
                    ss << "\tsetne\t%al\n";
                    ss << "_" << counter2 << ":\n";
                    counter_ = counter2 + 1;
                    break;
                }
                case eBinaryOp::LOr:
                {
                    auto[expr1, counter1] = emitExpression(*lOperand, counter);
                    ss << expr1;
                    ss << "\tcmpl\t$0, %eax\n";
                    ss << "\tje\t_" << counter1 << "\n";
                    ss << "\tmovl\t$1, %eax\n";
                    auto[expr2, counter2] = emitExpression(*rOperand, counter1 + 1);
                    ss << "\tjmp\t_" << counter2 << "\n";
                    ss << "_" << counter1 << ":\n";
                    ss << expr2;
                    ss << "\tcmpl\t$0, %eax\n";
                    ss << "\tmovl\t$0, %eax\n";
                    ss << "\tsetne\t%al\n";
                    ss << "_" << counter2 << ":\n";
                    counter_ = counter2 + 1;
                    break;
                }
                case eBinaryOp::Eq:
                {
                    auto[expr, counter1] = emitComp(*lOperand, *rOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\tsete\t%al\n";
                    break;
                }
                case eBinaryOp::Ne:
                {
                    auto[expr, counter1] = emitComp(*lOperand, *rOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\tsetne\t%al\n";
                    break;
                }
                case eBinaryOp::Lt:
                {
                    auto[expr, counter1] = emitComp(*lOperand, *rOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\tsetl\t%al\n";
                    break;
                }
                case eBinaryOp::Le:
                {
                    auto[expr, counter1] = emitComp(*lOperand, *rOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\tsetle\t%al\n";
                    break;
                }
                case eBinaryOp::Gt:
                {
                    auto[expr, counter1] = emitComp(*lOperand, *rOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\tsetg\t%al\n";
                    break;
                }
                case eBinaryOp::Ge:
                {
                    auto[expr, counter1] = emitComp(*lOperand, *rOperand, counter);
                    counter_ = counter1;
                    ss << expr;
                    ss << "\tsetge\t%al\n";
                    break;
                }
            }
        }

        return { ss.str(), counter_ };
    }

    std::string emitReturn(AST &ret)
    {
        std::stringstream ss;

        auto[expr] = std::get<Return>(std::move(ret.kind));

        ss << emitExpression(*expr).first;

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

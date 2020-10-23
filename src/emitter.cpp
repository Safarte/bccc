#include "parser.h"
#include "emitter.h"

#include <string>

namespace bccc
{
    std::string emitStatement(const Statement &statement)
    {
        std::stringstream ss;

        auto ret = statement.body;
        int val = ret.expression.body.value;

        ss << "\tmovl\t$" << val << ", %eax\n";
        ss << "\tret\n";

        return ss.str();
    }

    std::string emitProgram(const Program& program)
    {
        std::stringstream ss;

        auto funName = program.body.sName;

        ss << ".globl " << funName << "\n";
        ss << funName << ":\n";

        auto funBody = program.body.body;
        auto statementASM = emitStatement(funBody);

        ss << statementASM;

        return ss.str();
    }
}

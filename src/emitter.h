#ifndef BCCC_EMITTER_H
#define BCCC_EMITTER_H

#include "parser.h"

#include <string>

namespace bccc
{
    std::string emitStatement(const Statement &statement);
    std::string emitProgram(const Program &program);
}

#endif //BCCC_EMITTER_H

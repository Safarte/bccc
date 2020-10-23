#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "emitter.h"

#include <string>
#include <fstream>
#include <iostream>

namespace bccc
{
    void compile(const std::string &sInputFile, const std::string &sOutputFile)
    {
        auto tokens = Tokenize(sInputFile);

        auto ast = parseProgram(tokens);
        std::cout << ast << std::endl;

        auto assembly = emitProgram(ast);
        std::cout << assembly << std::endl;

        std::ofstream oFile;
        oFile.open(sOutputFile);
        oFile << assembly;
        oFile.close();
    }
}

#include "lexer.h"
#include "parser.h"
#include "emitter.h"

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::string fName{argv[1]};
    auto tokens = bccc::Tokenize(fName);

    for (auto t : tokens)
    {
        std::cout << t << " ";
    }
    std::cout << std::endl;

    auto ast = bccc::parseProgram(tokens);

    // std::cout << ast;

    auto assembly = bccc::emitProgram(ast);

    std::ofstream oFile;
    oFile.open("/tmp/asm.s");
    oFile << assembly;
    oFile.close();

    size_t fNameEnd = fName.find_last_of('.');
    std::string shortName = fName.substr(0, fNameEnd);

    std::string command = "gcc /tmp/asm.s -o ./" + shortName;
    std::system(command.c_str());

    return 0;
}
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
    auto ast = bccc::parseProgram(tokens);
    auto assembly = bccc::emitProgram(*ast);

//    std::cout << std::ifstream(fName, std::ios::in).rdbuf() << "\n\n";
//    for (auto t : tokens)
//    {
//        std::cout << t << " ";
//    }
//    std::cout << "\n\n" << *ast << "\n\n" << assembly;

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
#include "token.h"
#include "lexer.h"
#include "classifier.h"

#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>

int main(int argc, char **argv)
{
    auto vec = bccc::Tokenize("examples/return_2.c");

    for (auto x : vec)
    {
        auto it = std::find_if(bccc::classifiers.begin(), bccc::classifiers.end(), [&x](const bccc::Classifier c) { return c.type() == x.type; });
        std::cout << x.value << "->" << (*it).name() << "\n";
    }

    return 0;
}
#include "token.h"
#include "lexer.h"
#include "classifier.h"

#include <iostream>
#include <vector>

int main()
{
    auto vec = bccc::Tokenize("../examples/return_2.c");

    for (auto x : vec)
    {
        std::vector<bccc::Classifier> classifiers = bccc::getClassifiers();
        auto it = std::find_if(classifiers.begin(), classifiers.end(), [&x](const bccc::Classifier& c) { return c.type() == x.type; });
        std::cout << x.value << "->" << (*it).name() << "\n";
    }

    return 0;
}
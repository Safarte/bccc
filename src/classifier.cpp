#include <vector>
#include "classifier.h"

namespace bccc {
    std::vector<Classifier> getClassifiers() {
        std::vector<Classifier> classifiers{
                {Token::Type::Keyword,    "Keyword",    std::regex("int|return")},
                {Token::Type::Delimiter,  "Delimiter",  std::regex(R"({|}|\(|\)|;)", std::regex_constants::basic)},
                {Token::Type::Identifier, "Identifier", std::regex("[a-zA-Z][a-zA-Z0-9_]*",
                                                                   std::regex_constants::basic)},
                {Token::Type::IntLiteral, "IntLiteral", std::regex("0|[1-9][0-9]*", std::regex_constants::extended)},
                {Token::Type::HexLiteral, "HexLiteral", std::regex("0x[0-9a-f]+", std::regex_constants::extended)},
                {Token::Type::OctLiteral, "OctLiteral", std::regex("0[0-7]+", std::regex_constants::extended)},
        };
        return classifiers;
    }
}
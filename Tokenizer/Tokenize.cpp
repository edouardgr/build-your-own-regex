#include "Tokenize.h"

namespace Tokenizer
{
    std::vector<Token> Tokenize(const std::string& input)
    {
        std::vector<Token> tokens{ };
        for (int index = 0; index < input.length(); index++) {
            tokens.emplace_back(input[index]);
        }
        return tokens;
    }
}

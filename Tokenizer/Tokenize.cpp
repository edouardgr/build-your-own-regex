#include "Tokenize.h"

namespace Tokenizer
{
    std::vector<std::string> Tokenize(const std::string& input)
    {
        std::vector<std::string> tokens{ };
        for (const char c : input)
        {
            tokens.emplace_back(std::string { c });
        }
        return tokens;
    }
}

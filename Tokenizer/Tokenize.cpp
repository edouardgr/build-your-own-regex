#include "Tokenize.h"

namespace Tokenizer
{
    char constexpr LeftBracketCharacter = '[';
    char constexpr RightBracketCharacter = ']';

    size_t GetBracketLength(std::string const& input, size_t startIndex)
    {
        const size_t endIndex = input.find(RightBracketCharacter, startIndex);
        if (endIndex != std::string::npos)
            return endIndex - startIndex;

        throw std::invalid_argument("Cannot find closing bracket");
    }

    std::vector<std::string> Tokenize(const std::string& input)
    {
        std::vector<std::string> tokens{ };
        size_t index = 0;
        while (index < input.size())
        {
            if (input[index] == RightBracketCharacter)
                throw std::invalid_argument("Cannot find opening bracket");

            const size_t subStringLength = input[index] == LeftBracketCharacter ? GetBracketLength(input, index) + 1 : 1;
            tokens.emplace_back(std::string { input.substr(index, subStringLength) });
            index += subStringLength;
        }
        return tokens;
    }
}

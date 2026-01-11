#include "LiteralMatcher.h"

namespace LiteralMatcher
{
    MultipleLiteralMatcher::MultipleLiteralMatcher(const std::string& literal) : LiteralMatcher(Multiple)
    {
        const std::string trimmed = literal.substr(1, literal.length() - 2); // eat brackets
        for (size_t i = 0; i < trimmed.length(); ++i)
        {
            if (i + 1 < trimmed.length() && trimmed.at(i + 1) == '-')
            {
                Literals.push_back(std::make_unique<RangeLiteral>(trimmed[i], trimmed[i + 2]));
                i += 2;
                continue;
            }

            Literals.push_back(std::make_unique<SingleLiteral>(trimmed[i]));
        }
    }

    bool MultipleLiteralMatcher::IsMatching(const char character)
    {
        for (const auto& literal : Literals)
        {
            if (literal->Match(character))
            {
                return true;
            }
        }
        return false;
    }
}

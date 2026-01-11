#include "LiteralMatcher.h"

namespace LiteralMatcher
{
    MultipleLiteralMatcher::MultipleLiteralMatcher(const std::string& literal) : LiteralMatcher(Multiple)
    {
        for (size_t i = 0; i < literal.length(); ++i)
        {
            if (i + 1 < literal.length() && literal.at(i + 1) == '-')
            {
                Literals.push_back(std::make_unique<RangeLiteral>(literal[i], literal[i + 2]));
                i += 2;
                continue;
            }

            Literals.push_back(std::make_unique<SingleLiteral>(literal[i]));
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

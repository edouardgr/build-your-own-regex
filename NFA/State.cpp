#include "State.h"

namespace NFA
{
    void State::AddTransition(std::unique_ptr<LiteralMatcher::LiteralMatcher> matcher, const size_t nextStateIndex)
    {
        Transitions.emplace_back( std::move(matcher), nextStateIndex );
    }
}

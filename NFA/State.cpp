#include "State.h"
#include <iostream>

namespace NFA
{
    void State::AddTransition(const char character, const size_t nextStateIndex)
    {
        if (TransitionTable.contains(character))
        {
            TransitionTable[character].push_back(nextStateIndex);
        }
        else
        {
            TransitionTable.insert({character, std::vector { nextStateIndex }});
        }
    }
}

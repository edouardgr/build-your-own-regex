#include "NFA.h"
#include "NFABuilder.h"
#include <stack>

namespace FiniteAutomata
{
    NFA::NFA(AST::Node* rootNode)
    {
        auto [startStateIndex, _] = NFABuilder::DetermineProcess(rootNode, States, true);
        InitialStateIndex = startStateIndex;
    }

    bool NFA::Validate(const std::string& input) const
    {
        std::stack<std::pair<size_t, size_t>> stack;
        stack.emplace(std::pair { InitialStateIndex, 0 });

        while (!stack.empty())
        {
            auto [stateIndex, stringIndex] = stack.top();
            stack.pop();

            if (States[stateIndex].IsFinalState() && stringIndex == input.length())
            {
                return true;
            }

            for (const auto& [matcher, nextStateIndex] : States[stateIndex].GetTransitions())
            {
                if (matcher->GetType() == LiteralMatcher::Epsilon)
                {
                    stack.emplace( nextStateIndex, stringIndex );
                }
                else if (stringIndex < input.length() && matcher->IsMatching(input[stringIndex]))
                {
                    stack.emplace( nextStateIndex, stringIndex + 1 );
                }
            }
        }

        return false;
    }


}
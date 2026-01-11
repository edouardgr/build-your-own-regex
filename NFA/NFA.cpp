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

    struct StackData
    {
        size_t StateIndex;
        size_t StringIndex;
    };

    bool NFA::Validate(const std::string& input) const
    {
        std::stack<StackData> stack;
        stack.push({ InitialStateIndex, 0 });

        size_t currentStateIndex = InitialStateIndex;
        size_t currentStringIndex = 0;
        while (!stack.empty())
        {
            auto [stateIndex, stringIndex] = stack.top();
            currentStateIndex = stateIndex;
            currentStringIndex = stringIndex;
            stack.pop();

            const auto& transitions = States[currentStateIndex].GetTransitions();
            for (const auto& [matcher, nextStateIndex] : transitions)
            {
                if (matcher->GetType() == LiteralMatcher::Epsilon || (stringIndex < input.length() && matcher->IsMatching(input[stringIndex]))) // stringIndex (2) < input.length() (2)
                {
                    if (States[nextStateIndex].IsFinalState() && stringIndex == input.length())
                    {
                        return true;
                    }

                    const bool isEpsilon = matcher->GetType() == LiteralMatcher::Epsilon;
                    const size_t offset = isEpsilon ? 0 : 1;

                    stack.push({ nextStateIndex, stringIndex + offset });
                }
            }
        }

        return States[currentStateIndex].IsFinalState() && currentStringIndex == input.length();
    }
}

//          Alternation(|)
//	 	   /              \
// 	   Star(*)          Star(*)
//		  |                |
//	   Concat           Literal('c')
//	   /    \
// Literal   Star(*)
//  ('a')      |
// 		   Literal('b')

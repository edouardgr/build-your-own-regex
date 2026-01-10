#include "NFA.h"
#include "Constants.h"
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
        while (!stack.empty())
        {
            auto [stateIndex, stringIndex] = stack.top();
            currentStateIndex = stateIndex;
            stack.pop();

            auto transitionTable = States[currentStateIndex].GetTransitionTable();
            for (const auto& [character, nextStateIndexes] : transitionTable)
            {
                if (character == ::NFA::EpsilonCharacter)
                {
                    for (const size_t nextStateIndex : nextStateIndexes)
                    {
                        if (States[nextStateIndex].IsFinalState())
                        {
                            return true;
                        }

                        stack.push({ nextStateIndex, stringIndex });
                    }
                }

                if (stringIndex < input.length() && character == input[stringIndex])
                {
                    for (const size_t nextStateIndex : nextStateIndexes)
                    {
                        if (States[nextStateIndex].IsFinalState())
                        {
                            return true;
                        }

                        stack.push({ nextStateIndex, stringIndex + 1 });
                    }
                }
            }
        }

        return States[currentStateIndex].IsFinalState();
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

#include "NFA.h"

#include <stack>

namespace FiniteAutomata
{
    NFA::NFA(AST::Node* rootNode)
    {
        auto [startStateIndex, _] = DetermineProcess(rootNode, States, true);
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
                if (character == epsilonCharacter)
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

    void NFA::Print()
    {
        std::cout << "StartIndex: " << InitialStateIndex << std::endl << std::endl;
        for (size_t index = 0; index < States.size(); index++)
        {
            if  (States[index].IsFinalState())
            {
                std::cout << "Index " << index << " is a Final State" << std::endl;
            }
            else
            {
                std::cout << "Index " << index << std::endl;
                States[index].PrintTransitions();
                std::cout << std::endl;
            }
        }
    };
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

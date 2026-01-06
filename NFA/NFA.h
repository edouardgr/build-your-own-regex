#pragma once
#include <iostream>
#include <map>
#include <utility>
#include "../AST/AST.h"

namespace FiniteAutomata
{
    class State
    {
    public:
        explicit State(const bool isFinal) : IsFinal(isFinal) { TransitionTable = std::map<char, std::vector<size_t>> { }; }

        bool IsFinalState() const { return IsFinal; }

        void AddTransition(const char character, const size_t nextStateIndex)
        {
            if (TransitionTable.contains(character))
            {
                TransitionTable[character].push_back(nextStateIndex);
            }

            TransitionTable.insert({character, std::vector<size_t> { nextStateIndex }});
        }

        std::map<char, std::vector<size_t>> TransitionTable;

    private:
        bool IsFinal = false;
    };

    class NFA
    {
    public:
        explicit NFA(AST::Node* rootNode);

        void Print()
        {
            std::cout << "StartIndex: " << InitialStateIndex << std::endl;
            for (size_t index = 0; index < States.size(); index++)
            {
                for (const auto& [character, targetIndexes] : States[index].TransitionTable)
                {
                    for (const auto target_index : targetIndexes)
                    {
                        std::cout << "Index: " << index << ", transition: " << character << " -> " << target_index << std::endl;
                    }
                }

                if  (States[index].IsFinalState())
                {
                    std::cout << "Index " << index << " is a Final State" << std::endl;
                }
            }
        }

    private:
        std::vector<State> States;
        size_t InitialStateIndex;
    };
}

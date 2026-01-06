#pragma once
#include <iostream>
#include <map>
#include "../AST/AST.h"

namespace FiniteAutomata
{
    class State
    {
    public:
        explicit State(const bool isFinal) : IsFinal(isFinal) { TransitionTable = std::map<char, std::vector<size_t>> { }; }
        bool IsFinalState() const { return IsFinal; }
        void AddTransition(char character, size_t nextStateIndex);
        void PrintTransitions();

    private:
        bool IsFinal = false;
        std::map<char, std::vector<size_t>> TransitionTable;
    };

    class NFA
    {
    public:
        explicit NFA(AST::Node* rootNode);
        void Print();

    private:
        std::vector<State> States;
        size_t InitialStateIndex;
    };
}

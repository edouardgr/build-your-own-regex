#pragma once
#include <iostream>

#include "State.h"
#include "../AST/AST.h"

namespace FiniteAutomata
{
    class NFA
    {
    public:
        explicit NFA(AST::Node* rootNode);
        [[nodiscard]] bool Validate(const std::string& input) const;

    private:
        std::vector<::NFA::State> States;
        size_t InitialStateIndex;
    };
}

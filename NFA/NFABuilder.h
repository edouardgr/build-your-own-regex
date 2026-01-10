#pragma once

namespace NFABuilder
{
    struct Result
    {
        size_t startStateIndex;
        size_t endStateIndex;
    };

    Result DetermineProcess(AST::Node* node, std::vector<::NFA::State>& states, bool hasFinal = false);
}
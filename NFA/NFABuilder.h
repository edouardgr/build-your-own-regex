#pragma once

namespace NFABuilder
{
    std::pair<size_t, size_t> DetermineProcess(AST::Node* node, std::vector<::NFA::State>& states, bool hasFinal = false);
}
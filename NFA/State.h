#pragma once
#include "LiteralMatcher.h"

namespace NFA
{
    class State
    {
    public:
        explicit State(const bool isFinal) : IsFinal(isFinal) { }
        [[nodiscard]] bool IsFinalState() const { return IsFinal; }
        void AddTransition(std::unique_ptr<LiteralMatcher::LiteralMatcher> matcher, size_t nextStateIndex);
        [[nodiscard]] const std::vector<std::pair<std::unique_ptr<LiteralMatcher::LiteralMatcher>, size_t>>& GetTransitions() const { return Transitions; }

    private:
        bool IsFinal = false;
        std::vector<std::pair<std::unique_ptr<LiteralMatcher::LiteralMatcher>, size_t>> Transitions;
    };
}

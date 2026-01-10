#pragma once
#include <map>
#include <vector>

namespace NFA
{
    class State
    {
    public:
        explicit State(const bool isFinal) : IsFinal(isFinal) { TransitionTable = std::map<char, std::vector<size_t>> { }; }
        [[nodiscard]] bool IsFinalState() const { return IsFinal; }
        void AddTransition(char character, size_t nextStateIndex);
        [[nodiscard]] const std::map<char, std::vector<size_t>>& GetTransitionTable() const { return TransitionTable; };

    private:
        bool IsFinal = false;
        // TODO: Replace `char` with class + character range
        std::map<char, std::vector<size_t>> TransitionTable;
    };
}

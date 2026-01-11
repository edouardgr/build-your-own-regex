#pragma once
#include <memory>
#include <string>

#include "NFA/NFA.h"

class Regex
{
public:
    explicit Regex(const std::string& pattern);
    ~Regex() = default;

    [[nodiscard]] bool Validate(const std::string& input) const;

private:
    std::unique_ptr<FiniteAutomata::NFA> Nfa;
};

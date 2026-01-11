#include "Regex.h"

#include "AST/Parser.h"
#include "Lexer/Lexer.h"

Regex::Regex(const std::string& pattern)
{
    const std::vector<std::string> tokens = Lexer::Scan(pattern);
    AST::Parser parser{tokens};
    const std::unique_ptr<AST::Node> tree = parser.Parse();
    Nfa = std::make_unique<FiniteAutomata::NFA>(tree.get());
}

bool Regex::Validate(const std::string& input) const
{
    return Nfa->Validate(input);
}

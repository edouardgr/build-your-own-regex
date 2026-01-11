#include "State.h"
#include "../AST/AST.h"
#include "NFABuilder.h"

namespace NFABuilder
{
    static std::unique_ptr<LiteralMatcher::LiteralMatcher> CreateLiteralMatcher(const std::string& literal)
    {
        if (literal.length() > 1)
        {
            return std::make_unique<LiteralMatcher::MultipleLiteralMatcher>(literal);
        }

        return std::make_unique<LiteralMatcher::SingleLiteralMatcher>(literal[0]);
    }

    static std::pair<size_t, size_t> ProcessLiteral(const AST::LiteralNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        //        a
        // -->(q)-->(f)-->

        // Start index
        const size_t startIndex = states.size();
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size();
        states.emplace_back(hasFinal);

        states[startIndex].AddTransition(CreateLiteralMatcher(node->GetLiteral()), endIndex);

        return { startIndex, endIndex };
    }

    static std::pair<size_t, size_t> ProcessZeroOrMore(const AST::KleeneNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        //             -<<-
        //        ε   / ε  \   ε
        // -->(q)-->() N(a) ()-->(f)-->
        //      \       ε        /
        //       ------->>------

        // Start index
        const size_t startIndex = states.size();
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size();
        states.emplace_back(hasFinal);

        states[startIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), endIndex);

        auto [repeatStartIndex, repeatEndIndex] = DetermineProcess(node->GetLeftNode(), states, false);

        states[startIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), repeatStartIndex);
        states[repeatEndIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), repeatStartIndex);
        states[repeatEndIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), endIndex);

        return { startIndex, endIndex };
    }

    static std::pair<size_t, size_t> ProcessConcatenation(const AST::ConcatenationNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        // -->(q) N(a) () N(b) (f)-->

        // Start index
        const size_t originalStartIndex = states.size();
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size();
        states.emplace_back(hasFinal);

        size_t startIndex = originalStartIndex;
        for (const auto& child : node->GetChildren())
        {
            auto [childStartIndex, childEndIndex] = DetermineProcess(child.get(), states, false);

            states[startIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), childStartIndex);
            startIndex = childEndIndex;
        }

        states[startIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), endIndex);
        return { originalStartIndex, endIndex };
    }

    static std::pair<size_t, size_t> ProcessAlternation(const AST::AlternationNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        //        () N(a) ()
        //      ε/          \ε
        // -->(q)            (f)-->
        //      ε\          /ε
        //        () N(b) ()

        // Start index
        const size_t startIndex = states.size();
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size();
        states.emplace_back(hasFinal);

        auto [leftStartIndex, leftEndIndex] = DetermineProcess(node->GetLeftNode(), states, false); // 2, 3
        auto [rightStartIndex, rightEndIndex] = DetermineProcess(node->GetRightNode(), states, false); // 4, 5

        states[startIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), leftStartIndex);
        states[startIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), rightStartIndex);

        states[leftEndIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), endIndex);
        states[rightEndIndex].AddTransition(std::make_unique<LiteralMatcher::EpsilonMatcher>(), endIndex);

        return { startIndex, endIndex };
    }

    static std::pair<size_t, size_t> ProcessZeroOrOne(const AST::ZeroOrOneNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        // TODO:
        throw std::logic_error("Not yet implemented");
    }

    static std::pair<size_t, size_t> ProcessOneOrMore(const AST::OneOrMoreNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        // TODO:
        throw std::logic_error("Not yet implemented");
     }

    static std::pair<size_t, size_t> ProcessWildcard(const AST::WildcardNode* _, std::vector<NFA::State>& states, const bool hasFinal)
    {
        //        .
        // -->(q)-->(f)-->

        // Start index
        const size_t startIndex = states.size();
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size();
        states.emplace_back(hasFinal);

        states[startIndex].AddTransition(std::make_unique<LiteralMatcher::WildcardMatcher>(), endIndex);

        return { startIndex, endIndex };
    }

    std::pair<size_t, size_t> DetermineProcess(AST::Node* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        switch (node->GetType())
        {
        case AST::Literal:
            return ProcessLiteral(dynamic_cast<AST::LiteralNode*>(node), states, hasFinal);
        case AST::ZeroOrMore:
            return ProcessZeroOrMore(dynamic_cast<AST::KleeneNode*>(node), states, hasFinal);
        case AST::Concatenation:
            return ProcessConcatenation(dynamic_cast<AST::ConcatenationNode*>(node), states, hasFinal);
        case AST::Alternation:
            return ProcessAlternation(dynamic_cast<AST::AlternationNode*>(node), states, hasFinal);
        case AST::ZeroOrOne:
            return ProcessZeroOrOne(dynamic_cast<AST::ZeroOrOneNode*>(node), states, hasFinal);
        case AST::OneOrMore:
            return ProcessOneOrMore(dynamic_cast<AST::OneOrMoreNode*>(node), states, hasFinal);
        case AST::Wildcard:
            return ProcessWildcard(dynamic_cast<AST::WildcardNode*>(node), states, hasFinal);
        default:
            throw std::invalid_argument("Could not determine type");
        }
    }
}
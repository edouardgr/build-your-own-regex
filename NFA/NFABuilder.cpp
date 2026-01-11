#include "State.h"
#include "Constants.h"
#include "../AST/AST.h"
#include "NFABuilder.h"

namespace NFABuilder
{
    static Result ProcessLiteral(const AST::LiteralNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        //        a
        // -->(q)-->(f)-->

        // Start index
        const size_t startIndex = states.size();
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size();
        states.emplace_back(hasFinal);

        states[startIndex].AddTransition(node->GetLiteral(), endIndex);

        return Result { startIndex, endIndex };
    }

    static Result ProcessZeroOrMore(const AST::KleeneNode* node, std::vector<NFA::State>& states, const bool hasFinal)
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

        states[startIndex].AddTransition(NFA::EpsilonCharacter, endIndex);

        auto [repeatStartIndex, repeatEndIndex] = DetermineProcess(node->GetLeftNode(), states, false);

        states[startIndex].AddTransition(NFA::EpsilonCharacter, repeatStartIndex);
        states[repeatEndIndex].AddTransition(NFA::EpsilonCharacter, repeatStartIndex);
        states[repeatEndIndex].AddTransition(NFA::EpsilonCharacter, endIndex);

        return Result { startIndex, endIndex };
    }

    static Result ProcessConcatenation(const AST::ConcatenationNode* node, std::vector<NFA::State>& states, const bool hasFinal)
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

            states[startIndex].AddTransition(NFA::EpsilonCharacter, childStartIndex);
            startIndex = childEndIndex;
        }

        states[startIndex].AddTransition(NFA::EpsilonCharacter, endIndex);
        return Result { originalStartIndex, endIndex };
    }

    static Result ProcessAlternation(const AST::AlternationNode* node, std::vector<NFA::State>& states, const bool hasFinal)
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

        states[startIndex].AddTransition(NFA::EpsilonCharacter, leftStartIndex);
        states[startIndex].AddTransition(NFA::EpsilonCharacter, rightStartIndex);

        states[leftEndIndex].AddTransition(NFA::EpsilonCharacter, endIndex);
        states[rightEndIndex].AddTransition(NFA::EpsilonCharacter, endIndex);

        return Result { startIndex, endIndex };
    }

    static Result ProcessZeroOrOne(const AST::ZeroOrOneNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        // TODO:
    }

    static Result ProcessOneOrMore(const AST::OneOrMoreNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        // TODO:
    }

    static Result ProcessWildcard(const AST::WildcardNode* node, std::vector<NFA::State>& states, const bool hasFinal)
    {
        // TODO:
    }

    Result DetermineProcess(AST::Node* node, std::vector<NFA::State>& states, const bool hasFinal)
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
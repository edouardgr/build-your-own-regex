#include "NFA.h"

namespace FiniteAutomata
{
    constexpr char epsilonCharacter = '.';

    struct Result
    {
        size_t startStateIndex;
        size_t endStateIndex;
    };

    Result DetermineProcess(AST::Node* node, std::vector<State>& states, bool hasFinal = false);

    static Result ProcessLiteral(const AST::LiteralNode* node, std::vector<State>& states, const bool hasFinal)
    {
        //        a
        // -->(q)-->(f)-->

        // Start index
        const size_t startIndex = states.size();
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size();
        states.emplace_back(hasFinal);
        
        states[startIndex].AddTransition(node->GetCharacter(), endIndex);

        return Result { startIndex, endIndex };
    }

    static Result ProcessStar(AST::StarNode* node, std::vector<State>& states, const bool hasFinal)
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

        states[startIndex].AddTransition(epsilonCharacter, endIndex);

        auto [repeatStartIndex, repeatEndIndex] = DetermineProcess(node->GetLeftNode(), states, false);

        states[startIndex].AddTransition(epsilonCharacter, repeatStartIndex);
        states[repeatEndIndex].AddTransition(epsilonCharacter, repeatStartIndex);
        states[repeatEndIndex].AddTransition(epsilonCharacter, endIndex);

        return Result { startIndex, endIndex };
    }

    static Result ProcessConcatenation(AST::ConcatenationNode* node, std::vector<State>& states, const bool hasFinal)
    {
        // -->(q) N(a) () N(b) (f)-->

        // Start index
        const size_t originalStartIndex = states.size();
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size();
        states.emplace_back(hasFinal);

        size_t startIndex = originalStartIndex;
        for (const auto child : node->GetChildren())
        {
            auto [childStartIndex, childEndIndex] = DetermineProcess(child, states, false);

            states[startIndex].AddTransition(epsilonCharacter, childStartIndex);
            startIndex = childEndIndex;
        }

        states[startIndex].AddTransition(epsilonCharacter, endIndex);
        return Result { originalStartIndex, endIndex };
    }

    static Result ProcessAlternation(AST::AlternationNode* node, std::vector<State>& states, const bool hasFinal)
    {
        //        () N(a) ()
        //      ε/          \ε
        // -->(q)            (f)-->
        //      ε\          /ε
        //        () N(b) ()

        // Start index
        const size_t startIndex = states.size(); // 0
        states.emplace_back(false);

        // End index
        const size_t endIndex = states.size(); // 1
        states.emplace_back(hasFinal);

        auto [leftStartIndex, leftEndIndex] = DetermineProcess(node->GetLeftNode(), states, false); // 2, 3
        auto [rightStartIndex, rightEndIndex] = DetermineProcess(node->GetRightNode(), states, false); // 4, 5

        states[startIndex].AddTransition(epsilonCharacter, leftStartIndex); // 0 -> 2
        states[startIndex].AddTransition(epsilonCharacter, rightStartIndex); // 0 -> 3

        states[leftEndIndex].AddTransition(epsilonCharacter, endIndex);
        states[rightEndIndex].AddTransition(epsilonCharacter, endIndex);

        return Result { startIndex, endIndex };
    }

    Result DetermineProcess(AST::Node* node, std::vector<State>& states, const bool hasFinal)
    {
        switch (node->GetType())
        {
        case AST::Literal:
            return ProcessLiteral(dynamic_cast<AST::LiteralNode*>(node), states, hasFinal);
        case AST::Star:
            return ProcessStar(dynamic_cast<AST::StarNode*>(node), states, hasFinal);
        case AST::Concatenation:
            return ProcessConcatenation(dynamic_cast<AST::ConcatenationNode*>(node), states, hasFinal);
        case AST::Alternation:
            return ProcessAlternation(dynamic_cast<AST::AlternationNode*>(node), states, hasFinal);
        default:
            throw std::invalid_argument("Could not determine type");
        }
    }

    NFA::NFA(AST::Node* rootNode)
    {
        auto [startStateIndex, _] = DetermineProcess(rootNode, States, true);
        InitialStateIndex = startStateIndex;
    };
}

//          Alternation(|)
//	 	   /              \
// 	   Star(*)          Star(*)
//		  |                |
//	   Concat           Literal('c')
//	   /    \
// Literal   Star(*)
//  ('a')      |
// 		   Literal('b')

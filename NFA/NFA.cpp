#include "NFA.h"

#include <stack>

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
        
        states[startIndex].AddTransition(node->GetLiteral(), endIndex);

        return Result { startIndex, endIndex };
    }

    static Result ProcessStar(const AST::KleeneNode* node, std::vector<State>& states, const bool hasFinal)
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

    static Result ProcessConcatenation(const AST::ConcatenationNode* node, std::vector<State>& states, const bool hasFinal)
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

            states[startIndex].AddTransition(epsilonCharacter, childStartIndex);
            startIndex = childEndIndex;
        }

        states[startIndex].AddTransition(epsilonCharacter, endIndex);
        return Result { originalStartIndex, endIndex };
    }

    static Result ProcessAlternation(const AST::AlternationNode* node, std::vector<State>& states, const bool hasFinal)
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
        case AST::ZeroOrMore:
            return ProcessStar(dynamic_cast<AST::KleeneNode*>(node), states, hasFinal);
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
    }

    struct StackData
    {
        size_t StateIndex;
        size_t StringIndex;
    };

    bool NFA::Validate(const std::string& input) const
    {
        std::stack<StackData> stack;
        stack.push({ InitialStateIndex, 0 });

        size_t currentStateIndex = InitialStateIndex;
        while (!stack.empty())
        {
            auto [stateIndex, stringIndex] = stack.top();
            currentStateIndex = stateIndex;
            stack.pop();

            auto transitionTable = States[currentStateIndex].GetTransitionTable();
            for (const auto& [character, nextStateIndexes] : transitionTable)
            {
                if (character == epsilonCharacter)
                {
                    for (const size_t nextStateIndex : nextStateIndexes)
                    {
                        if (States[nextStateIndex].IsFinalState())
                        {
                            return true;
                        }

                        stack.push({ nextStateIndex, stringIndex });
                    }
                }

                if (stringIndex < input.length() && character == input[stringIndex])
                {
                    for (const size_t nextStateIndex : nextStateIndexes)
                    {
                        if (States[nextStateIndex].IsFinalState())
                        {
                            return true;
                        }

                        stack.push({ nextStateIndex, stringIndex + 1 });
                    }
                }
            }
        }

        return States[currentStateIndex].IsFinalState();
    }

    void NFA::Print()
    {
        std::cout << "StartIndex: " << InitialStateIndex << std::endl << std::endl;
        for (size_t index = 0; index < States.size(); index++)
        {
            if  (States[index].IsFinalState())
            {
                std::cout << "Index " << index << " is a Final State" << std::endl;
            }
            else
            {
                std::cout << "Index " << index << std::endl;
                States[index].PrintTransitions();
                std::cout << std::endl;
            }
        }
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

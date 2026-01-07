#include "AST/AST.h"
#include "Tokenizer/Token.h"
#include "Tokenizer/Tokenize.h"
#include <iostream>
#include <memory>
#include <vector>

#include "NFA/NFA.h"

int main()
{
    // Tokenize input
    const std::string inputRegex = "(ab*)*|c*";
    const std::vector<Tokenizer::Token> tokens = Tokenizer::Tokenize(inputRegex);

    // Tokens -> AST
    AST::Parser parser{ tokens };
    const std::unique_ptr<AST::Node> tree = parser.Parse();
    std::cout << tree->Print() << std::endl << std::endl;

    // AST -> NFA
    FiniteAutomata::NFA nfa { tree.get() };
    nfa.Print();

    std::cout << nfa.Validate("c");
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

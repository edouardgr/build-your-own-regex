#include "AST/AST.h"
#include "AST/Parser.h"
#include "Tokenizer/Tokenize.h"
#include "NFA/NFA.h"
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    // a? - Zero or one
    // a+ - One or more
    // . - wildcard
    // [] - character range

    // Tokenize input
    const std::string inputRegex = "(ab*)*|c*[xyz]";
    const std::vector<std::string> tokens = Tokenizer::Tokenize(inputRegex);

    // Tokens -> AST
    AST::Parser parser { tokens };
    const std::unique_ptr<AST::Node> tree = parser.Parse();
    std::cout << tree->Print() << std::endl;

    // AST -> NFA
    FiniteAutomata::NFA nfa { tree.get() };
    // nfa.Print();

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

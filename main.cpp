#include "AST/AST.h"
#include "AST/Parser.h"
#include "Tokenizer/Tokenize.h"
#include "NFA/NFA.h"
#include <iostream>
#include <memory>
#include <vector>

#include "Regex.h"

int main()
{
    // a? - Zero or one
    // a+ - One or more
    // . - wildcard
    // [] - character range

    const auto regex = Regex("(ab*)*|c*[xyz]");
    std::cout << regex.Validate("c");
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

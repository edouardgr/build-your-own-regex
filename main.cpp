#include <iostream>

#include "Regex.h"

int main()
{
    // a - Literal
    // a|b - Or
    // ab - Concatenation
    // a* - Zero or many
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

#include <iostream>

#include "Regex.h"

int main()
{
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

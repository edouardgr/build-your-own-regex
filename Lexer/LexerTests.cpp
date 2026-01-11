#include <catch2/catch_test_macros.hpp>

#include "Lexer.h"
#include "catch2/matchers/catch_matchers.hpp"

namespace LexerTests
{
    TEST_CASE ("Lexer - Input string is tokenized")
    {
        REQUIRE( Lexer::Scan("a*|+?.") == std::vector<std::string>{ "a", "*", "|", "+", "?", "." });
        REQUIRE( Lexer::Scan("[abc]") == std::vector<std::string>{ "[abc]" });
        REQUIRE( Lexer::Scan("[a-z]") == std::vector<std::string>{ "[a-z]" });
        REQUIRE( Lexer::Scan("[]yz") == std::vector<std::string>{ "[]", "y", "z" });
    }

    TEST_CASE ("Lexer - Input string is incorrect")
    {
        REQUIRE_THROWS_AS( Lexer::Scan("]["), std::invalid_argument);
        REQUIRE_THROWS_AS( Lexer::Scan("["), std::invalid_argument);
    }
}

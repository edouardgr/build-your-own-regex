#include <catch2/catch_test_macros.hpp>

#include "Tokenize.h"
#include "catch2/matchers/catch_matchers.hpp"

namespace TokenizerTests
{
    TEST_CASE ("Input string is tokenized")
    {
        REQUIRE( Tokenizer::Tokenize("a*|+?.") == std::vector<std::string>{ "a", "*", "|", "+", "?", "." });
        REQUIRE( Tokenizer::Tokenize("[abc]") == std::vector<std::string>{ "[abc]" });
        REQUIRE( Tokenizer::Tokenize("[a-z]") == std::vector<std::string>{ "[a-z]" });
        REQUIRE( Tokenizer::Tokenize("[]yz") == std::vector<std::string>{ "[]", "y", "z" });
    }

    TEST_CASE ("Input string is incorrect")
    {
        REQUIRE_THROWS_AS( Tokenizer::Tokenize("]["), std::invalid_argument);
        REQUIRE_THROWS_AS( Tokenizer::Tokenize("["), std::invalid_argument);
    }
}

#include <catch2/catch_test_macros.hpp>

#include "Regex.h"

namespace RegexTests
{
    TEST_CASE("Literal - matches single character")
    {
        const Regex regex("a");
        REQUIRE(regex.Validate("a") == true);
        REQUIRE(regex.Validate("b") == false);
        REQUIRE(regex.Validate("") == false);
        REQUIRE(regex.Validate("aa") == false);
    }

    TEST_CASE("Or - matches either alternative")
    {
        const Regex regex("a|b");
        REQUIRE(regex.Validate("a") == true);
        REQUIRE(regex.Validate("b") == true);
        REQUIRE(regex.Validate("c") == false);
        REQUIRE(regex.Validate("ab") == false);
        REQUIRE(regex.Validate("") == false);
    }

    TEST_CASE("Concatenation - matches sequence")
    {
        const Regex regex("ab");
        REQUIRE(regex.Validate("ab") == true);
        REQUIRE(regex.Validate("a") == false);
        REQUIRE(regex.Validate("b") == false);
        REQUIRE(regex.Validate("ba") == false);
        REQUIRE(regex.Validate("abc") == false);
        REQUIRE(regex.Validate("") == false);
    }

    TEST_CASE("Zero or many - matches zero or more occurrences")
    {
        const Regex regex("a*");
        REQUIRE(regex.Validate("") == true);
        REQUIRE(regex.Validate("a") == true);
        REQUIRE(regex.Validate("aa") == true);
        REQUIRE(regex.Validate("aaa") == true);
        REQUIRE(regex.Validate("b") == false);
        REQUIRE(regex.Validate("ab") == false);
    }

    TEST_CASE("Zero or one - matches zero or one occurrence")
    {
        const Regex regex("a?");
        REQUIRE(regex.Validate("") == true);
        REQUIRE(regex.Validate("a") == true);
        REQUIRE(regex.Validate("aa") == false);
        REQUIRE(regex.Validate("b") == false);
    }

    TEST_CASE("One or more - matches one or more occurrences")
    {
        const Regex regex("a+");
        REQUIRE(regex.Validate("") == false);
        REQUIRE(regex.Validate("a") == true);
        REQUIRE(regex.Validate("aa") == true);
        REQUIRE(regex.Validate("aaa") == true);
        REQUIRE(regex.Validate("b") == false);
        REQUIRE(regex.Validate("ab") == false);
    }

    TEST_CASE("Wildcard - matches any single character")
    {
        const Regex regex(".");
        REQUIRE(regex.Validate("a") == true);
        REQUIRE(regex.Validate("b") == true);
        REQUIRE(regex.Validate("1") == true);
        REQUIRE(regex.Validate("") == false);
        REQUIRE(regex.Validate("ab") == false);
    }

    TEST_CASE("Character range - matches characters in range")
    {
        SECTION("Explicit characters [abc]")
        {
            const Regex regex("[abc]");
            REQUIRE(regex.Validate("a") == true);
            REQUIRE(regex.Validate("b") == true);
            REQUIRE(regex.Validate("c") == true);
            REQUIRE(regex.Validate("d") == false);
            REQUIRE(regex.Validate("") == false);
            REQUIRE(regex.Validate("ab") == false);
        }

        SECTION("Range notation [a-z]")
        {
            const Regex regex("[a-z]");
            REQUIRE(regex.Validate("a") == true);
            REQUIRE(regex.Validate("m") == true);
            REQUIRE(regex.Validate("z") == true);
            REQUIRE(regex.Validate("A") == false);
            REQUIRE(regex.Validate("1") == false);
            REQUIRE(regex.Validate("") == false);
        }

        SECTION("Range notation [0-9]")
        {
            const Regex regex("[0-9]");
            REQUIRE(regex.Validate("0") == true);
            REQUIRE(regex.Validate("4") == true);
            REQUIRE(regex.Validate("9") == true);
            REQUIRE(regex.Validate("A") == false);
            REQUIRE(regex.Validate("") == false);
        }
    }

    TEST_CASE("Combined patterns")
    {
        SECTION("Concatenation with quantifiers")
        {
            const Regex regex("a*b");
            REQUIRE(regex.Validate("b") == true);
            REQUIRE(regex.Validate("ab") == true);
            REQUIRE(regex.Validate("aab") == true);
            REQUIRE(regex.Validate("aaab") == true);
            REQUIRE(regex.Validate("a") == false);
            REQUIRE(regex.Validate("") == false);
        }

        SECTION("Or with concatenation")
        {
            const Regex regex("ab|cd");
            REQUIRE(regex.Validate("ab") == true);
            REQUIRE(regex.Validate("cd") == true);
            REQUIRE(regex.Validate("ac") == false);
            REQUIRE(regex.Validate("a") == false);
        }

        SECTION("Wildcard with quantifier")
        {
            const Regex regex(".*");
            REQUIRE(regex.Validate("") == true);
            REQUIRE(regex.Validate("a") == true);
            REQUIRE(regex.Validate("abc") == true);
            REQUIRE(regex.Validate("hello world") == true);
        }
    }
}

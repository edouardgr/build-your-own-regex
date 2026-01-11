#pragma once
#include <memory>
#include <string>
#include <vector>

namespace LiteralMatcher
{
    enum LiteralType
    {
        Single,
        Multiple,
        Wildcard,
        Epsilon,
    };

    class Literal
    {
    public:
        virtual ~Literal() = default;
        virtual bool Match(char character) = 0;
    };

    class SingleLiteral : public Literal
    {
    public:
        explicit SingleLiteral(const char character) : Literal(character) { };
        bool Match(const char character) override { return Literal == character; };
    private:
        char Literal;
    };

    class RangeLiteral : public Literal
    {
    public:
        explicit RangeLiteral(const char minCharacter, const char maxCharacter) : MinCharacter(minCharacter), MaxCharacter(maxCharacter) { }
        bool Match(const char character) override { return MinCharacter <= character && character <= MaxCharacter; };
    private:
        char MinCharacter;
        char MaxCharacter;
    };

    class LiteralMatcher
    {
    public:
        explicit LiteralMatcher(const LiteralType type) : Type(type) { };
        virtual ~LiteralMatcher() = default;
        virtual bool IsMatching(char character) = 0;
        [[nodiscard]] LiteralType GetType() const { return Type; };
    private:
        LiteralType Type;
    };

    class SingleLiteralMatcher : public LiteralMatcher
    {
    public:
        explicit SingleLiteralMatcher(const char literal) : LiteralMatcher(Single), Literal(literal) { };
        bool IsMatching(const char character) override { return Literal == character; };
    private:
        char Literal;
    };

    class MultipleLiteralMatcher : public LiteralMatcher
    {
    public:
        explicit MultipleLiteralMatcher(const std::string& literal);
        bool IsMatching(char character) override;
    private:
        std::vector<std::unique_ptr<Literal>> Literals;
    };

    class WildcardMatcher : public LiteralMatcher
    {
    public:
        explicit WildcardMatcher() : LiteralMatcher(Wildcard) { }
        bool IsMatching(char character) override { return true; }
    };

    class EpsilonMatcher : public LiteralMatcher
    {
    public:
        explicit EpsilonMatcher() : LiteralMatcher(Epsilon) { };
        bool IsMatching(char character) override { return true; }
    };
}

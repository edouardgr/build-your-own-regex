#pragma once

namespace Tokenizer
{
    class Token
    {
    public:
        explicit Token(const char character) : Character{ character } {}
        char GetCharacter() const { return Character; }

    private:
        char Character;
    };
}
#include "Parser.h"
#include "Constants.h"

namespace AST
{
    std::string Parser::Peek() const
    {
        if (pos >= Tokens.size())
        {
            throw std::out_of_range("Consuming token out of range");
        }
        return Tokens[pos];
    }

    std::string Parser::Consume()
    {
        if (pos >= Tokens.size())
        {
            throw std::out_of_range("Peeking token out of range");
        }
        return Tokens[pos++];
    }

    std::unique_ptr<Node> Parser::Parse()
    {
        return ParseAlternation();
    }

    std::unique_ptr<Node> Parser::ParsePrimary()
    {
        const std::string token = Peek();

        if (token[0] == OpenParenthesesCharacter)
        {
            Consume(); // eat (
            auto expression = ParseAlternation();
            Consume(); // eat )
            return expression;
        }

        if (token[0] == WildcardCharacter)
        {
            Consume(); // eat .
            return std::make_unique<WildcardNode>();
        }

        Consume(); // eat literal
        return std::make_unique<LiteralNode>(token);
    }

    std::unique_ptr<Node> Parser::ParseKleene()
    {
        auto left = ParsePrimary();

        if (HasRemaining() && Peek()[0] == ZeroOrMoreCharacter)
        {
            Consume(); // eat *
            return std::make_unique<ZeroOrMoreNode>(std::move(left));
        }

        if (HasRemaining() && Peek()[0] == ZeroOrOneCharacter)
        {
            Consume(); // eat ?
            return std::make_unique<ZeroOrOneNode>(std::move(left));
        }

        if (HasRemaining() && Peek()[0] == OneOrMoreCharacter)
        {
            Consume(); // eat +
            return std::make_unique<OneOrMoreNode>(std::move(left));
        }

        return left;
    }

    std::unique_ptr<Node> Parser::ParseConcatenation()
    {
        std::vector<std::unique_ptr<Node>> nodes{ };

        // Keep parsing star until we hit | or ) or end
        while (HasRemaining() && Peek()[0] != AlternationCharacter && Peek()[0] != CloseParenthesesCharacter)
        {
            nodes.push_back(ParseKleene());
        }

        if (nodes.size() == 1)
        {
            return std::move(nodes[0]);
        }

        return std::make_unique<ConcatenationNode>(std::move(nodes));
    }

    std::unique_ptr<Node> Parser::ParseAlternation()
    {
        auto left = ParseConcatenation();

        if (HasRemaining() && Peek()[0] == AlternationCharacter)
        {
            Consume(); // eat |
            auto right = ParseAlternation();
            return std::make_unique<AlternationNode>(std::move(left), std::move(right));
        }

        return left;
    }
} // AST
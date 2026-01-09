#include "Parser.h"
#include "Contants.h"

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

        Consume(); // consume literal
        return std::make_unique<LiteralNode>(token[0]);
    }

    std::unique_ptr<Node> Parser::ParseStar()
    {
        auto left = ParsePrimary();

        if (HasRemaining() && Peek()[0] == StarCharacter)
        {
            Consume(); // eat *
            return std::make_unique<StarNode>(std::move(left));
        }

        return left;
    }

    std::unique_ptr<Node> Parser::ParseConcatenation()
    {
        std::vector<std::unique_ptr<Node>> nodes{ };

        // Keep parsing star until we hit | or ) or end
        while (HasRemaining() && Peek()[0] != AlternationCharacter && Peek()[0] != CloseParenthesesCharacter)
        {
            nodes.push_back(ParseStar());
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
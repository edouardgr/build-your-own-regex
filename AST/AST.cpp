#include "AST.h"
#include <stack>

namespace AST
{
	std::string AlternationNode::Print()
	{
		return "(" + LeftNode->Print() + "|" + RightNode->Print() + ")";
	}

	std::string ConcatenationNode::Print()
	{
		std::string result = "(";
		for (const auto& child : Children)
		{
			result += child->Print();
		}
		result += ")";
		return result;
	}

	std::string StarNode::Print()
	{
		return LeftNode->Print() + "*";
	}

	std::string LiteralNode::Print()
	{
		return std::string { Character };
	}

	Tokenizer::Token Parser::Peek() const
	{
		if (pos >= Tokens.size())
		{
			throw std::out_of_range("Peeking token out of range");
		}
		return Tokens[pos];
	}

	Tokenizer::Token Parser::Consume()
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
		const Tokenizer::Token token = Peek();

		if (token.GetCharacter() == OpenParenthesesCharacter)
		{
			Consume(); // eat (
			auto expression = ParseAlternation();
			Consume(); // eat )
			return expression;
		}

		Consume(); // consume literal
		return std::make_unique<LiteralNode>(token.GetCharacter());
	}

	std::unique_ptr<Node> Parser::ParseStar()
	{
		auto left = ParsePrimary();

		if (HasRemaining() && Peek().GetCharacter() == StarCharacter)
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
		while (HasRemaining() && Peek().GetCharacter() != AlternationCharacter && Peek().GetCharacter() != CloseParenthesesCharacter)
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

		if (HasRemaining() && Peek().GetCharacter() == AlternationCharacter)
		{
			Consume(); // eat |
			auto right = ParseAlternation();
			return std::make_unique<AlternationNode>(std::move(left), std::move(right));
		}

		return left;
	}
}
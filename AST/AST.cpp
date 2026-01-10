#include "AST.h"

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

	std::string LiteralNode::Print()
	{
		return std::string { _literal };
	}

	std::string WildcardNode::Print()
	{
		return ".";
	}

	std::string ZeroOrMoreNode::Print()
	{
		return LeftNode->Print() + "*";
	}

	std::string ZeroOrOneNode::Print()
	{
		return LeftNode->Print() + "?";
	}

	std::string OneOrMoreNode::Print()
	{
		return LeftNode->Print() + "+";
	}
}

#include "AST.h"

namespace AST
{
	std::string AlternationNode::Print()
	{
		return "(" + LeftNode->Print() + "|" + RightNode->Print() + ")";
	}

	ConcatenationNode::ConcatenationNode(std::vector<std::unique_ptr<Node>> children): Node(Concatenation)
	{
		Children = std::move(children);
	}

	std::vector<Node*> ConcatenationNode::GetChildren() const
	{
		std::vector<Node*> children;
		children.reserve(Children.size());

		for (auto& child : Children)
		{
			children.emplace_back(child.get());
		}

		return children;
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
		return std::string { _literal };
	}
}

#pragma once
#include "../Tokenizer/Token.h"
#include "../Tokenizer/Tokenize.h"
#include <memory>
#include <utility>
#include <vector>

namespace AST
{
    constexpr char AlternationCharacter = '|';
    constexpr char StarCharacter = '*';
    constexpr char OpenParenthesesCharacter = '(';
    constexpr char CloseParenthesesCharacter = ')';

    enum NodeType
    {
        Literal, // 'a' 'b'
        Star, // *
        Concatenation, // ab
        Alternation // |
    };

    class Node {
    public:
        explicit Node(const NodeType type) : Type{ type } {}
        virtual ~Node() = default;

        NodeType GetType() const { return Type; }
        virtual std::string Print() { return "Error"; };

    private:
        NodeType Type;
    };

    class AlternationNode : public Node {
    public:
        explicit AlternationNode(std::unique_ptr<Node> leftNode, std::unique_ptr<Node> rightNode) : Node(Alternation),
            LeftNode{std::move(leftNode)}, RightNode{std::move(rightNode)}
        {
        }

        Node* GetLeftNode() const { return LeftNode.get(); };
        Node* GetRightNode() const { return RightNode.get(); };

        std::string Print() override;

    private:
        std::unique_ptr<Node> LeftNode;
        std::unique_ptr<Node> RightNode;
    };

    class ConcatenationNode : public Node {
    public:
        explicit ConcatenationNode(std::vector<std::unique_ptr<Node>> children) : Node(Concatenation)
        {
            while (!children.empty())
            {
                Children.push_back(std::move(children[0]));
                children.erase(children.begin());
            }
        }

        std::vector<Node*> GetChildren() const
        {
            std::vector<Node*> children;
            children.reserve(Children.size());

            for (auto& child : Children)
            {
                children.emplace_back(child.get());
            }

            return children;
        };

        std::string Print() override;

    private:
        std::vector<std::unique_ptr<Node>> Children;
    };

    class StarNode : public Node {
    public:
        explicit StarNode(std::unique_ptr<Node> leftNode) : Node(Star), LeftNode {std::move(leftNode)} {}
        std::string Print() override;

        Node* GetLeftNode() const { return LeftNode.get(); };

    private:
        std::unique_ptr<Node> LeftNode;
    };

    class LiteralNode : public Node {
    public:
        explicit LiteralNode(const char character) : Node(Literal), Character { character } {}
        char GetCharacter() const { return Character; }
        std::string Print() override;

    private:
        char Character;
    };

    class Parser {
    public:
        explicit Parser(const std::vector<Tokenizer::Token>& tokens) : Tokens{ tokens } {};
        std::unique_ptr<Node> Parse();

    private:
        std::vector<Tokenizer::Token> Tokens;
        int pos = 0;

        bool HasRemaining() const { return pos < Tokens.size(); };
        Tokenizer::Token Peek() const;
        Tokenizer::Token Consume();
        std::unique_ptr<Node> ParsePrimary();
        std::unique_ptr<Node> ParseStar();
        std::unique_ptr<Node> ParseConcatenation();
        std::unique_ptr<Node> ParseAlternation();
    };
}
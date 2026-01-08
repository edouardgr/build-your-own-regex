#pragma once
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

        [[nodiscard]] NodeType GetType() const { return Type; }
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

        [[nodiscard]] Node* GetLeftNode() const { return LeftNode.get(); };
        [[nodiscard]] Node* GetRightNode() const { return RightNode.get(); };

        std::string Print() override;

    private:
        std::unique_ptr<Node> LeftNode;
        std::unique_ptr<Node> RightNode;
    };

    class ConcatenationNode : public Node {
    public:
        explicit ConcatenationNode(std::vector<std::unique_ptr<Node>> children);
        [[nodiscard]] std::vector<Node*> GetChildren() const;;
        std::string Print() override;

    private:
        std::vector<std::unique_ptr<Node>> Children;
    };

    class StarNode : public Node {
    public:
        explicit StarNode(std::unique_ptr<Node> leftNode) : Node(Star), LeftNode {std::move(leftNode)} {}
        std::string Print() override;

        [[nodiscard]] Node* GetLeftNode() const { return LeftNode.get(); };

    private:
        std::unique_ptr<Node> LeftNode;
    };

    class LiteralNode : public Node {
    public:
        explicit LiteralNode(const char character) : Node(Literal), _literal { character } {}
        [[nodiscard]] char GetLiteral() const { return _literal; }
        std::string Print() override;

    private:
        char _literal;
    };

    class Parser {
    public:
        explicit Parser(const std::vector<std::string>& tokens) : Tokens{ tokens } {};
        std::unique_ptr<Node> Parse();

    private:
        std::vector<std::string> Tokens;
        int pos = 0;

        [[nodiscard]] bool HasRemaining() const { return pos < Tokens.size(); };
        [[nodiscard]] std::string Peek() const;
        std::string Consume();
        std::unique_ptr<Node> ParsePrimary();
        std::unique_ptr<Node> ParseStar();
        std::unique_ptr<Node> ParseConcatenation();
        std::unique_ptr<Node> ParseAlternation();
    };
}
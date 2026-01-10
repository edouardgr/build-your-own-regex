#pragma once
#include <memory>
#include <utility>
#include <vector>

namespace AST
{
    enum NodeType
    {
        Literal, // 'a' 'b' '[a-z]'
        Concatenation, // ab
        Alternation, // |
        ZeroOrMore, // *
        ZeroOrOne, // ?
        OneOrMore, // +
        Wildcard, // .
    };

    // Node (abstract)
    // - AlternationNode
    // - ConcatenationNode
    // - LiteralNode
    // - WildcardNode
    // - KleeneNode (abstract)
    //   - ZeroOrMoreNode
    //   - ZeroOrOneNode
    //   - OneOrMoreNode

    class Node {
    public:
        explicit Node(const NodeType type) : Type{ type } {}
        virtual ~Node() = default;

        [[nodiscard]] NodeType GetType() const { return Type; }
        virtual std::string Print() { throw std::logic_error("Function not yet implemented"); };

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
        explicit ConcatenationNode(std::vector<std::unique_ptr<Node>> children) : Node(Concatenation), Children(std::move(children)) { }
        [[nodiscard]] const std::vector<std::unique_ptr<Node>>& GetChildren() const { return Children; }
        std::string Print() override;

    private:
        std::vector<std::unique_ptr<Node>> Children;
    };

    class LiteralNode : public Node {
    public:
        explicit LiteralNode(const char character) : Node(Literal), _literal { character } {}
        [[nodiscard]] char GetLiteral() const { return _literal; }
        std::string Print() override;

    private:
        char _literal;
    };

    class WildcardNode : public Node
    {
    public:
        explicit WildcardNode() : Node(Wildcard) {}
        std::string Print() override;
    };

    class KleeneNode : public Node {
    public:
        explicit KleeneNode(const NodeType type, std::unique_ptr<Node> leftNode) : Node(type), LeftNode {std::move(leftNode)} {}
        std::string Print() override { throw std::logic_error("Function not yet implemented"); };

        [[nodiscard]] Node* GetLeftNode() const { return LeftNode.get(); };

    protected:
        std::unique_ptr<Node> LeftNode;
    };

    class ZeroOrMoreNode : public KleeneNode
    {
    public:
        explicit ZeroOrMoreNode(std::unique_ptr<Node> leftNode) : KleeneNode(ZeroOrMore, std::move(leftNode)) {}
        std::string Print() override;
    };

    class ZeroOrOneNode : public KleeneNode
    {
    public:
        explicit ZeroOrOneNode(std::unique_ptr<Node> leftNode) : KleeneNode(ZeroOrOne, std::move(leftNode)) {}
        std::string Print() override;
    };

    class OneOrMoreNode : public KleeneNode
    {
    public:
        explicit OneOrMoreNode(std::unique_ptr<Node> leftNode) : KleeneNode(OneOrMore, std::move(leftNode)) {}
        std::string Print() override;
    };
}
#pragma once
#include <memory>
#include <vector>

#include "AST.h"

namespace AST
{
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

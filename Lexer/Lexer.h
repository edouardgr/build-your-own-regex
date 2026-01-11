#pragma once
#include <memory>
#include <vector>

namespace Lexer
{
    std::vector<std::string> Scan(const std::string& input);
}
#pragma once
#include "Token.h"
#include <memory>
#include <vector>

namespace Tokenizer
{
    std::vector<Token> Tokenize(const std::string& input);
}
#pragma once
#include <utility>
#include <sstream>

namespace RegularExpression {

    enum class TokenType {
        Character,
        EndOfFile,
        LeftParenthesis,
        Quantifier,
        RightParenthesis,
    };

    using Token = std::pair<TokenType, std::string>;

    Token Tokenize(std::stringstream& source);

};

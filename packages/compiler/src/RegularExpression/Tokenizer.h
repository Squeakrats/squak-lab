#pragma once
#include <utility>
#include<sstream>

namespace RegularExpression {

enum class TokenType {
	Character,
	Alternate,
	Quantifier,
	LeftBracket,
	RightBracket,
	LeftParenthesis,
	RightParenthesis,
	EndOfFile
};

using Token = std::pair<TokenType, std::string>;

Token Tokenize(std::stringstream& source);

}
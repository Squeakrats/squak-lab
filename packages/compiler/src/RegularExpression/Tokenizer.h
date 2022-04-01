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

class Tokenizer {
private:
	std::stringstream source;

public:
	Tokenizer(std::string source) : source(source) {};

	Token Next();
};

}
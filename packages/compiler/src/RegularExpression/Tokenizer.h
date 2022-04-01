#pragma once
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

struct Token {
	TokenType type{};
	std::string text{};
};

class Tokenizer {
private:
	std::stringstream source;

public:
	Tokenizer(std::string source) : source(source) {};

	Token Next();
};

}
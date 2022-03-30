#pragma once 
#include <sstream>
#include <assert.h>

namespace BackusNaur {

enum class TokenType {
	Symbol,
	Replaces,
	Literal,
	SemiColon,
	Alternate,
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

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

using Token = std::pair<TokenType, std::string>;

class Tokenizer {
private:
	std::stringstream source;

public:
	Tokenizer(std::string source) : source(source) {};

	Token Next();
};

}

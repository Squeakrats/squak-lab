#pragma once 
#include <sstream>
#include <assert.h>

class BNFTokenizer {
public:
	const enum class TokenType {
		Symbol,
		Replaces,
		Terminal,
		NewLine,
		Alternate,
		EndOfFile
	};

	struct Token {
		TokenType type{};
		std::string text{};
	};

private:
	std::stringstream& source;

public:
	BNFTokenizer(std::stringstream& source) : source(source) {};

	Token Next();
};

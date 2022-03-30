#pragma once 
#include <sstream>
#include <assert.h>

class BNFTokenizer {
public:
	const enum class TokenType {
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

private:
	std::stringstream source;

public:
	BNFTokenizer(std::string source) : source(source) {};

	Token Next();
};

#pragma once 
#include <sstream>
#include <assert.h>

enum class TokenType {
	Symbol,
	Replaces,
	SemiColon,
	Code,
	Alternate,
	EndOfFile
};

using Token = std::pair<TokenType, std::string>;

Token Tokenize(std::stringstream& source);
